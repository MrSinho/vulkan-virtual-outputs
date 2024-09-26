#ifndef VVO_H
#define VVO_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus



#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <shvulkan/shVulkan.h>
#include <mongoose/mongoose.h>


typedef uint8_t VvoBool;

typedef struct mg_mgr             MongoEventManager;
typedef struct mg_connection      MongoConnection;
typedef struct mg_http_message    MongoHttpMessage;
typedef struct mg_ws_message      MongoWebSocketMessage;
typedef struct mg_http_serve_opts MongoHttpServeOptions;



#define VVO_TRUE  (VvoBool)1
#define VVO_FALSE (VvoBool)0

#define VVO_HTTP_REPLY "HTTP/1.1"
#define VVO_HTTP_OK    "200 OK\n"

#define VVO_HTTP_OK_INT        200
#define VVO_HTTP_NOT_FOUND_INT 400
#define VVO_HTTP_ERROR_INT     500

#define VVO_HTML_CONTENT_TYPE_HEADER  "Content-Type: text/html\r\n"
#define VVO_PNG_CONTENT_TYPE_HEADER   "Content-Type: image/png\r\n"
#define VVO_PNG_CONTENT_LENGTH_HEADER "Content-Length: %u\r\n"
#define VVO_END_HTTP_REPLY            "\r\n"



#define vvoError(condition, msg, failure_expression) \
    if ((int)(condition)) { printf("vvo error: %s.\n", msg); failure_expression; }

#define vvoMessage(condition, msg) \
    if ((int)(condition)) { printf("vvo message: %s.\n", msg); }



static char* squares = "\
<!DOCTYPE html>\
<html>\
<body>\
\
<h2>The feOffset and feBlend Filters</h2>\
\
<svg height = \"150\" width = \"150\" xmlns = \"http://www.w3.org/2000/svg\">\
<defs>\
<filter id = \"f1\" width = \"120\" height = \"120\">\
<feOffset in = \"SourceGraphic\" dx = \"20\" dy = \"20\"/>\
<feBlend in = \"SourceGraphic\" in2 = \"offOut\"/>\
</filter>\
</defs>\
<rect width = \"90\" height = \"90\" stroke = \"green\" stroke - width = \"3\" fill = \"yellow\" filter = \"url(#f1)\"/>\
Sorry, your browser does not support inline SVG.\
</svg>\
\
</body>\
</html>\
\
";

static char* square = "\
<!DOCTYPE html>\
<html>\
<body>\
\
<svg height=\"110\" width=\"110\" xmlns=\"http://www.w3.org/2000/svg\">\
<defs>\
<filter id = \"f1\">\
<feDropShadow dx = \"12\" dy = \"14\" stdDeviation = \"1\" flood - opacity = \"0.7\"/>\
</filter>\
</defs>\
<rect width = \"90\" height = \"90\" fill = \"yellow\" filter = \"url(#f1)\"/>\
</svg>\
\
</body>\
</html>\
";

static char* image = "\
<!DOCTYPE html>\
<html>\
<body>\
\
<img src=\"./output.png\">\
\
</body>\
</html>\
";

static char* web_socket_http_client = "<!DOCTYPE html>\
<html>\
<body>\
\
<img id=\"image\"/>\
\
<script src=\"js/web_socket_http_client.js\"></script>\
\
</body>\
</html>\
";



#define VVO_MAX_SRC_IMAGE_COUNT 6

typedef struct VvoHandle {

    VkPhysicalDevice physical_device;
	VkDevice         device;

	uint32_t surface_width;
	uint32_t surface_height;

	uint32_t supported_color_formats_count;

	VkFormat supported_color_formats[SH_MAX_STACK_DEVICE_COLOR_FORMATS_QUERIES];
	uint32_t single_channels_sizes  [SH_MAX_STACK_DEVICE_COLOR_FORMATS_QUERIES];
	uint32_t channels_count         [SH_MAX_STACK_DEVICE_COLOR_FORMATS_QUERIES];
	uint32_t channels_types         [SH_MAX_STACK_DEVICE_COLOR_FORMATS_QUERIES];

	uint32_t src_image_count;

	VkFormat src_image_format;
	uint32_t src_image_channel_size;
	uint32_t src_image_channel_count;
	uint32_t src_image_channel_type;
    uint32_t src_image_size;

	VkImage        src_images       [VVO_MAX_SRC_IMAGE_COUNT];
	VkDeviceMemory src_images_memory[VVO_MAX_SRC_IMAGE_COUNT];
	VkImageView    src_image_views  [VVO_MAX_SRC_IMAGE_COUNT];

	VkAttachmentDescription color_attachment;
	VkAttachmentReference   color_attachment_reference;

	VkAttachmentDescription depth_attachment;
	VkAttachmentReference   depth_attachment_reference;

	VkImage             dst_image;
	VkDeviceMemory      dst_image_memory;

	VkSubresourceLayout dst_image_subresource_layout;
	uint32_t            dst_image_offset;
	uint8_t*            p_dst_image_data;

	VkFence     image_copy_fence;
	VkSemaphore image_copy_semaphore;

	uint8_t* p_stbi_image_data;
	
	uint32_t png_image_size;
	uint8_t* p_png_image_data;

	uint32_t image_submissions_count;

	MongoEventManager event_manager;
    MongoConnection*  p_connection;

} VvoHandle;



extern uint8_t vvoInit(
	uint32_t         src_image_count,
	uint32_t         surface_width,
	uint32_t         surface_height,
	VvoHandle*       p_vvo
);

extern uint8_t vvoVulkanInit(
	VkDevice         device,
	VkPhysicalDevice physical_device,
	VvoHandle*       p_vvo
);

extern uint8_t vvoFindSupportedDeviceColorFormats(
	VvoHandle* p_vvo
);

extern uint8_t vvoCreateSrcImages(
	VvoHandle* p_vvo
);

extern uint8_t vvoCreateRenderpassColorAttachment(
	VvoHandle* p_vvo,
	uint32_t   attachment_idx
);

extern uint8_t vvoCreateDstImage(
	VvoHandle* p_vvo
);

extern uint8_t vvoCopyToDstImage(
	VvoHandle*       p_vvo,
	VkCommandBuffer  transfer_cmd_buffer,
	uint32_t         graphics_queue_family_index,
	uint32_t         transfer_queue_family_index,
	uint32_t         src_image_idx
);

extern uint8_t vvoSubmitImageCopy(
	VvoHandle*       p_vvo,
	VkCommandBuffer  transfer_cmd_buffer,
	VkQueue          transfer_queue,
	VkSemaphore      graphics_queue_finished_semaphore
);

extern uint8_t vvoFormatDstImageData(
	VvoHandle* p_vvo
);

extern uint8_t vvoReadDstImage(
	VvoHandle* p_vvo
);

extern uint8_t vvoGetDstImagePngData(
	VvoHandle* p_vvo
);

extern uint8_t vvoWriteDstImageToDisk(
	VvoHandle* p_vvo,
	char*      path
);

extern uint8_t vvoFreeStbiImageData(
	VvoHandle* p_vvo
);

extern uint8_t vvoSetupServer(
    VvoHandle* p_vvo,
    char*      uri
);

extern uint8_t vvoPollEvents(
    VvoHandle* p_vvo
);

extern uint8_t vvoMainLoop(
    VvoHandle* p_vvo,
    uint8_t*   p_loop_condition
);

extern uint8_t vvoVulkanRelease(
	VvoHandle* p_vvo
);

extern uint8_t vvoRelease(
    VvoHandle* p_vvo
);

extern void vvoHandleEvents(
    MongoConnection* p_connection,
    int              event,
    void*            event_data
);

extern void vvoSleepMs(
    uint32_t milliseconds
);

extern uint8_t vvoReadBinary(
    const char* path,
    uint32_t*   p_size,
    uint8_t**   pp_data
);



#ifdef __cplusplus
}
#endif//_cplusplus

#endif//VVO_H