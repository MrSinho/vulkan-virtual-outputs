#ifndef VVO_H
#define VVO_H



#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <vulkan/vulkan.h>
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

#define VVO_MAX_SWAPCHAIN_IMAGE_COUNT 6


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



typedef enum VvoSupportedImageFormat {
    
    //Signed 1d floating point numbers
    VVO_FORMAT_R16_SFLOAT = VK_FORMAT_R16_SFLOAT,
    VVO_FORMAT_R32_SFLOAT = VK_FORMAT_R32_SFLOAT,
    VVO_FORMAT_R64_SFLOAT = VK_FORMAT_R64_SFLOAT,

    //Signed 2d floating point numbers
    VVO_FORMAT_R16G16_SFLOAT = VK_FORMAT_R16G16_SFLOAT,
    VVO_FORMAT_R32G32_SFLOAT = VK_FORMAT_R32G32_SFLOAT,
    VVO_FORMAT_R64G64_SFLOAT = VK_FORMAT_R64G64_SFLOAT,

    //Signed 3d floating point numbers
    VVO_FORMAT_R16G16B16_SFLOAT = VK_FORMAT_R16G16B16_SFLOAT,
    VVO_FORMAT_R32G32B32_SFLOAT = VK_FORMAT_R32G32B32_SFLOAT,
    VVO_FORMAT_R64G64B64_SFLOAT = VK_FORMAT_R64G64B64_SFLOAT,

    //Unsigned 1d integers
    VVO_FORMAT_R8_UINT  = VK_FORMAT_R8_UINT,
    VVO_FORMAT_R16_UINT = VK_FORMAT_R16_UINT,
    VVO_FORMAT_R32_UINT = VK_FORMAT_R32_UINT,
    VVO_FORMAT_R64_UINT = VK_FORMAT_R64_UINT,

    //Unsigned 2d integers
    VVO_FORMAT_R8G8_UINT   = VK_FORMAT_R8G8_UINT,
    VVO_FORMAT_R16G16_UINT = VK_FORMAT_R16G16_UINT,
    VVO_FORMAT_R32G32_UINT = VK_FORMAT_R32G32_UINT,
    VVO_FORMAT_R64G64_UINT = VK_FORMAT_R64G64_UINT,

    //Unsigned 3d integers
    VVO_FORMAT_R8G8B8_UINT    = VK_FORMAT_R8G8B8_UINT,
    VVO_FORMAT_R16G16B16_UINT = VK_FORMAT_R16G16B16_UINT,
    VVO_FORMAT_R32G32B32_UINT = VK_FORMAT_R32G32B32_UINT,
    VVO_FORMAT_R64G64B64_UINT = VK_FORMAT_R64G64B64_UINT,

    //Signed 1d integers
    VVO_FORMAT_R8_SINT  = VK_FORMAT_R8_SINT,
    VVO_FORMAT_R16_SINT = VK_FORMAT_R16_SINT,
    VVO_FORMAT_R32_SINT = VK_FORMAT_R32_SINT,
    VVO_FORMAT_R64_SINT = VK_FORMAT_R64_SINT,

    //Signed 2d integers
    VVO_FORMAT_R8G8_SINT   = VK_FORMAT_R8G8_SINT,
    VVO_FORMAT_R16G16_SINT = VK_FORMAT_R16G16_SINT,
    VVO_FORMAT_R32G32_SINT = VK_FORMAT_R32G32_SINT,
    VVO_FORMAT_R64G64_SINT = VK_FORMAT_R64G64_SINT,

    //Signed 3d integers
    VVO_FORMAT_R8G8B8_SINT    = VK_FORMAT_R8G8B8_SINT,
    VVO_FORMAT_R16G16B16_SINT = VK_FORMAT_R16G16B16_SINT,
    VVO_FORMAT_R32G32B32_SINT = VK_FORMAT_R32G32B32_SINT,
    VVO_FORMAT_R64G64B64_SINT = VK_FORMAT_R64G64B64_SINT,

} VvoSupportedImageFormat;



typedef struct VvoSurface {

    uint32_t width;
    uint32_t height;
    
} VvoSurface;



typedef struct VvoSwapchain {

    VvoSurface               surface;
    VvoSupportedImageFormat  format;

    uint32_t swapchain_image_count;
    VvoBool  vsync;

    uint32_t scissors_x;
    uint32_t scissors_y;

    uint32_t image_extent_x;
    uint32_t image_extent_y;

    uint32_t images_sizes[VVO_MAX_SWAPCHAIN_IMAGE_COUNT];
    uint8_t* p_images[VVO_MAX_SWAPCHAIN_IMAGE_COUNT];


} VvoSwapchain;



typedef struct VvoHandle {

    VvoSwapchain      swapchain;
    MongoEventManager event_manager;
    MongoConnection*  p_connection;

} VvoHandle;



extern uint8_t vvoSetupServer(
    VvoHandle* p_handle,
    char* uri
);

extern uint8_t vvoPollEvents(
    VvoHandle* p_handle
);

extern uint8_t vvoMainLoop(
    VvoHandle* p_handle,
    uint8_t* p_loop_condition
);

extern uint8_t vvoRelease(
    VvoHandle* p_handle
);

extern void vvoHandleEvents(
    MongoConnection* p_connection,
    int              event,
    void* event_data
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