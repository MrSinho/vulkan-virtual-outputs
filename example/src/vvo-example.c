#ifdef __cplusplus
extern "C" {
#endif//_cplusplus



#include <stdio.h>

#include <vvo/vvo.h>



int main(void) {

	VvoHandle handle = { 0 };


	uint32_t width  = 400;
	uint32_t height = 400;

	vvoInit(1, width, height, &handle);

	vvoSetupServer(&handle, "http://127.0.0.1:8001");

	uint32_t i = 1;

	uint32_t src_image_channel_count = 3;
	uint32_t src_image_channel_size  = 1;
	uint32_t src_image_size          = width * height * src_image_channel_count * src_image_channel_size;

	uint8_t* p_stbi_image_data = calloc(src_image_size, 1);

	handle.src_image_channel_count = src_image_channel_count;
	handle.src_image_channel_size  = src_image_channel_size;
	handle.src_image_size          = src_image_size;
	handle.p_stbi_image_data       = p_stbi_image_data;
	
	vvoError(p_stbi_image_data == NULL, "main: invalid stbi image memory", return -1);

	while (1) {
		vvoPollEvents(&handle);

		for (uint32_t y = 0; y < height; y++) {
			for (uint32_t x = 0; x < width; x++) {

				int pixel_index = (y * width + x) * 3;

				uint8_t x_gradient = (uint8_t)(255 * x / width  * (i % 50) / 50);
				uint8_t y_gradient = (uint8_t)(255 * y / height * (i % 50) / 50);
				uint8_t r_gradient = (uint8_t)(255              * (i % 50) / 50);

				p_stbi_image_data[pixel_index + 0] = r_gradient;//Red
				p_stbi_image_data[pixel_index + 1] = x_gradient;//Green
				p_stbi_image_data[pixel_index + 2] = y_gradient;//Blue
			}
		}

		vvoGetDstImagePngData(&handle);

		i++;
	}

	vvoRelease(&handle);

	return 0;
}



#ifdef __cplusplus
}
#endif//_cplusplus

