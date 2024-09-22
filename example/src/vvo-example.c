#ifdef __cplusplus
extern "C" {
#endif//_cplusplus



#include <vvo/vvo.h>
#include <stdio.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>



int main(void) {

	VvoHandle handle = { 0 };


	uint32_t width = 400;
	uint32_t height = 400;

	
	vvoSetupServer(&handle, "http://127.0.0.1:8001");

	uint32_t i = 1;

	while (1) {
		vvoPollEvents(&handle);
		
		uint8_t* p_buffer = calloc(width * height * 3, 1);

		vvoError(p_buffer == NULL, "invalid buffer memory", return 1);

		for (uint32_t y = 0; y < height; y++) {
			for (uint32_t x = 0; x < width; x++) {

				int pixel_index = (y * width + x) * 3;

				uint8_t x_gradient = (uint8_t)((255.0f * x) / width)  * (float)((i % 50) / 50.0f);
				uint8_t y_gradient = (uint8_t)((255.0f * y) / height) * (float)((i % 50) / 50.0f);
				uint8_t r_gradient = (uint8_t)(255                    * ((float)(i % 50) / 50.0f));

				p_buffer[pixel_index + 0] = r_gradient;//Red
				p_buffer[pixel_index + 1] = x_gradient;//Green
				p_buffer[pixel_index + 2] = y_gradient;//Blue
			}
		}

		uint32_t image_size = 0;
		uint8_t* p_image = stbi_write_png_to_mem(p_buffer, width * 3, width, height, 3, &image_size);

		vvoError(p_image == NULL, "invalid image memory", return 1);

		handle.swapchain.images_sizes[0] = image_size;
		handle.swapchain.p_images[0] = p_image;

		i++;
	}

	vvoRelease(&handle);



	//uint32_t width  = 100;
	//uint32_t height = 100;
	//
	//VvoSurface surface = {
	//	.width  = width,
	//	.height = height
	//};
	//
	//VvoSupportedImageFormat image_format = VVO_FORMAT_R16G16B16_SFLOAT;
	//
	//VvoSwapchain swapchain = {
	//	.surface = surface,
	//	.format  = image_format,
	//
	//	.swapchain_image_count = 1,
	//	.vsync                 = VVO_FALSE,
	//	
	//	.scissors_x = width,
	//	.scissors_y = height,
	//	
	//	.image_extent_x = width,
	//	.image_extent_y = height,
	//};
	//
	//printf("Hello world");

	

	return 0;
}



#ifdef __cplusplus
}
#endif//_cplusplus

