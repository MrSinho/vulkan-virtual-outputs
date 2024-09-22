#ifdef __cplusplus
extern "C" {
#endif//__cplusplus



#include "vvo/vvo.h"


#ifdef _WIN32
#include <Windows.h>
#else
#include <termios.h>
#endif//_WIN32






uint8_t vvoSetupServer(
	VvoHandle* p_handle,
	char*      uri
) {
	vvoError(p_handle == NULL, "vvoPollEvents: invalid handle memory", return 0);

	mg_mgr_init(&p_handle->event_manager);

	mg_http_listen(&p_handle->event_manager, uri, vvoHandleEvents, p_handle);

	//p_connection->fn_data = p_handle;

	return 1;
}

uint8_t vvoPollEvents(
	VvoHandle* p_handle
) {
	vvoError(p_handle == NULL, "vvoPollEvents: invalid handle memory", return 0);

	mg_mgr_poll(&p_handle->event_manager, 1000);

	return 1;
}

uint8_t vvoMainLoop(
	VvoHandle* p_handle,
	uint8_t*   p_loop_condition
) {
	vvoError(p_handle         == NULL, "vvoMainLoop: invalid handle memory",         return 0);
	vvoError(p_loop_condition == NULL, "vvoMainLoop: invalid loop condition memory", return 0);

	while (*p_loop_condition) {
		vvoPollEvents(p_handle);
	}

	return 1;
}

uint8_t vvoRelease(
	VvoHandle* p_handle
) {
	vvoError(p_handle == NULL, "vvoRelease: invalid handle memory", return 0);

	VvoSwapchain* p_swapchain = &p_handle->swapchain;

	for (uint32_t swapchain_image_idx = 0; swapchain_image_idx < p_swapchain->swapchain_image_count; swapchain_image_idx++) {

		p_swapchain->images_sizes[swapchain_image_idx] = 0;
		
		uint8_t* p_image = p_swapchain->p_images[swapchain_image_idx];

		if (p_image != NULL) { 
			free(p_image);
		}

		mg_mgr_free(&p_handle->event_manager);

	}

	return 1;
}

void vvoHandleEvents(
	MongoConnection* p_connection,
	int              event, 
	void*            event_data
) {

	vvoMessage(event == MG_EV_HTTP_MSG, "vvoHandleEvents: http connection established");
	vvoMessage(event == MG_EV_WS_OPEN,  "vvoHandleEvents: websocket connection established");
	vvoMessage(event == MG_EV_WS_MSG,   "vvoHandleEvents: websocket message received");

	VvoHandle*    p_handle    = p_connection->fn_data;
	VvoSwapchain* p_swapchain = &p_handle->swapchain;

	MongoHttpMessage*      p_http_message      = NULL;
	MongoWebSocketMessage* p_websocket_message = NULL;

	if (event == MG_EV_HTTP_MSG) {//http connections

		p_http_message = (MongoHttpMessage*)event_data;

		vvoError(p_http_message == NULL, "invalid http message memory", return);

		uint8_t time_request     = mg_match(p_http_message->uri, mg_str("/api/time/get"),  NULL);
		uint8_t square_request   = mg_match(p_http_message->uri, mg_str("/square"),        NULL);
		uint8_t squares_request  = mg_match(p_http_message->uri, mg_str("/squares"),       NULL);
		uint8_t image_request    = mg_match(p_http_message->uri, mg_str("/static-image"),  NULL);
		uint8_t png_request      = mg_match(p_http_message->uri, mg_str("/output.png"),    NULL);
		uint8_t stream_request   = mg_match(p_http_message->uri, mg_str("/vvoStream"),     NULL);
		uint8_t stream_websocket = mg_match(p_http_message->uri, mg_str("/vvoStreamWS"),   NULL);

		//html content requests
		     if (square_request )  { mg_http_reply(p_connection, VVO_HTTP_OK_INT, VVO_HTML_CONTENT_TYPE_HEADER, square);  }
		else if (squares_request)  { mg_http_reply(p_connection, VVO_HTTP_OK_INT, VVO_HTML_CONTENT_TYPE_HEADER, squares); }
		else if (image_request)    { mg_http_reply(p_connection, VVO_HTTP_OK_INT, VVO_HTML_CONTENT_TYPE_HEADER, image);   }
		else if (stream_request)   { puts("stream request"); mg_http_reply(p_connection, VVO_HTTP_OK_INT, VVO_HTML_CONTENT_TYPE_HEADER, web_socket_http_client); }

		//redirected requests 
		else if (stream_websocket) { puts("going to websocket"); mg_ws_upgrade(p_connection, p_http_message, NULL); } //change connection to websocket

		//assets requests
		else if (png_request) {

			mg_printf(p_connection, 
				VVO_HTTP_REPLY " " VVO_HTTP_OK
				VVO_PNG_CONTENT_TYPE_HEADER
				VVO_PNG_CONTENT_LENGTH_HEADER
				VVO_END_HTTP_REPLY
				,
				p_swapchain->images_sizes[0]
			);

			mg_send(p_connection, p_swapchain->p_images[0], p_swapchain->images_sizes[0]);

		}
		else if (mg_match(p_http_message->uri, mg_str("/js/web_socket_http_client.js"), NULL)) {
			MongoHttpServeOptions opts = { .root_dir = NULL };
			mg_http_serve_file(p_connection, p_http_message, "js/web_socket_http_client.js", &opts);
		}
		else if (mg_match(p_http_message->uri, mg_str("/favicon.ico"), NULL)) {
		}

		else { 
			printf("Unsupported URI: %s", p_http_message->uri.buf);
			mg_http_reply(p_connection, VVO_HTTP_NOT_FOUND_INT, "", "{%m:%m}\n", MG_ESC("error"), MG_ESC("Unsupported URI")); 
		}

	}

	if (event == MG_EV_WS_MSG) {//websocket connections
		p_websocket_message = (MongoWebSocketMessage*)event_data;
		vvoError(p_websocket_message == NULL, "invalid websocket message memory", return);
	}
	
	if (p_connection->is_websocket) {//while the connection is websocket
		mg_ws_send(p_connection, p_swapchain->p_images[0], p_swapchain->images_sizes[0], WEBSOCKET_OP_BINARY);
		//vvoSleepMs(1);
	}
}



void vvoSleepMs(uint32_t milliseconds) {
#ifdef _WIN32
	Sleep((DWORD)milliseconds);
#else
	usleep(milliseconds * 1000);
#endif//_WIN32
}



#ifdef _MSC_VER
#pragma warning (disable: 4996)
#endif//_MSC_VER

uint8_t vvoReadBinary(
	const char* path,
	uint32_t*   p_size,
	uint8_t**   pp_data
) {

	FILE* stream = fopen(path, "rb");

	if (stream == NULL) {
		return 0;
	}

	fseek(stream, 0, SEEK_END);
	uint32_t code_size = ftell(stream);
	fseek(stream, 0, SEEK_SET);

	(*pp_data) = (uint8_t*)calloc(1, code_size);

	if ((*pp_data) == NULL) {
		fclose(stream);
		return 0;
	}

	fread((*pp_data), code_size, 1, stream);
	*p_size = code_size;

	fclose(stream);

	return 1;
}



#ifdef __cplusplus
}
#endif//__cplusplus