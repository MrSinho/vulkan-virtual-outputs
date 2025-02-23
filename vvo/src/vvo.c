#ifdef __cplusplus
extern "C" {
#endif//__cplusplus



#include <shvulkan/shVulkan.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#ifdef _MSC_VER
#pragma warning (disable: 4996)
#endif//_MSC_VER
#include <stb/stb_image_write.h>

#include "vvo/vvo.h"



uint8_t vvoInit(
	uint32_t         src_image_count,
	uint32_t         surface_width,
	uint32_t         surface_height,
	VvoHandle*       p_vvo
) {
	vvoError(p_vvo            == NULL,           "vvoInit: invalid vvo handle memory", return 0);
	vvoError(surface_width    == 0,              "vvoInit: invalid surface width",     return 0);
	vvoError(surface_height   == 0,              "vvoInit: invalid surface width",     return 0);

	p_vvo->surface_width   = surface_width;
	p_vvo->surface_height  = surface_height;
	p_vvo->src_image_count = src_image_count;

	return 1;
}

uint8_t vvoVulkanInit(
	VkDevice         device,
	VkPhysicalDevice physical_device,
	VvoHandle*       p_vvo
) {
	vvoError(p_vvo            == NULL,           "vvoInit: invalid vvo handle memory", return 0);
	vvoError(device           == VK_NULL_HANDLE, "vvoInit: invalid device memory",     return 0);
	vvoError(physical_device  == VK_NULL_HANDLE, "vvoInit: invalid physical memory",   return 0);

	p_vvo->device          = device;
	p_vvo->physical_device = physical_device;

	shCreateFences(
		device,//device
		1,//fence_count
		1,//signaled
		&p_vvo->image_copy_fence//p_fences
	);

	shCreateSemaphores(
		device,//device
		1,//semaphore_count
		&p_vvo->image_copy_semaphore//p_semaphores
	);

	return 1;
}

uint8_t vvoFindSupportedDeviceColorFormats(
	VvoHandle* p_vvo
) {
	vvoError(p_vvo == NULL, "vvoFindSupportedDeviceColorFormats: invalid vvo handle memory", return 0);

	shFindSupportedDeviceColorFormats(
		p_vvo->physical_device,//physical_device
		4,//min_channel_count
		4,//max_channel_count
		1,//min_channel_size
		8,//max_channel_size
		SH_IMAGE_CHANNEL_TYPE_UNORM | SH_IMAGE_CHANNEL_TYPE_SNORM,//channel_types
		&p_vvo->supported_color_formats_count,//p_supported_format_count
		p_vvo->supported_color_formats,//p_supported_formats
		p_vvo->channels_count,//p_channels_count
		p_vvo->single_channels_sizes,//p_single_channels_sizes
		p_vvo->channels_types//p_channels_types
	);

	shVkError(
		p_vvo->supported_color_formats_count == 0,
		"cannot find a supported image format",
		return 0
	);

	p_vvo->src_image_format        = p_vvo->supported_color_formats[0];
	p_vvo->src_image_channel_size  = p_vvo->single_channels_sizes[0];
	p_vvo->src_image_channel_count = p_vvo->channels_count[0];
	p_vvo->src_image_channel_type  = p_vvo->channels_types[0];

	p_vvo->src_image_size = p_vvo->surface_height * p_vvo->surface_width * p_vvo->src_image_channel_count * p_vvo->src_image_channel_size;

	return 1;
}

uint8_t vvoCreateSrcImages(
	VvoHandle* p_vvo
) {
	vvoError(p_vvo == NULL, "vvoCreateSrcImages: invalid vvo handle memory", return 0);

	for (uint32_t image_idx = 0; image_idx < p_vvo->src_image_count; image_idx++) {
		shCreateImage(
			p_vvo->device,//device
			VK_IMAGE_TYPE_2D,//type
			p_vvo->surface_width,//x
			p_vvo->surface_height,//y
			1,//z
			p_vvo->src_image_format,//format
			1,//mip_levels
			1,//sample_count
			VK_IMAGE_TILING_LINEAR,//image_tiling
			VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT,//usage
			VK_SHARING_MODE_EXCLUSIVE,//sharing_mode
			&p_vvo->src_images[image_idx]//p_image
		);

		shAllocateImageMemory(
			p_vvo->device,//device
			p_vvo->physical_device,//physical_device
			p_vvo->src_images[image_idx],//image
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,//memory_property_flags
			&p_vvo->src_images_memory[image_idx]//p_image_memory
		);

		shBindImageMemory(
			p_vvo->device,//device
			p_vvo->src_images[image_idx],//image
			0,//offset
			p_vvo->src_images_memory[image_idx]//image_memory
		);

		shCreateImageView(
			p_vvo->device,//device
			p_vvo->src_images[image_idx],//image
			VK_IMAGE_VIEW_TYPE_2D,//view_type
			VK_IMAGE_ASPECT_COLOR_BIT,//image_aspect
			1,//mip_levels
			p_vvo->src_image_format,//format
			&p_vvo->src_image_views[image_idx]//p_image_view
		);
	}

	return 1;
}

uint8_t vvoCreateRenderpassColorAttachment(
	VvoHandle* p_vvo,
	uint32_t   attachment_idx
) {
	vvoError(p_vvo == NULL, "vvoCreateColorAttachment: invalid vvo handle memory", return 0);

	VkAttachmentLoadOp load_treatment = VK_ATTACHMENT_LOAD_OP_CLEAR;

	if (attachment_idx > 0) {//is not input color attachment, but it's resolve attachment
		load_treatment = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	}

	shCreateRenderpassAttachment(
		p_vvo->src_image_format,//format
		1,//sample_count
		load_treatment,//load_treatment
		VK_ATTACHMENT_STORE_OP_STORE,//store_treatment
		VK_ATTACHMENT_LOAD_OP_DONT_CARE,//stencil_load_treatment
		VK_ATTACHMENT_STORE_OP_DONT_CARE,//stencil_store_treatment
		VK_IMAGE_LAYOUT_UNDEFINED,//initial_layout
		VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,//final_layout
		&p_vvo->color_attachment//p_attachment_description
	);

	shCreateRenderpassAttachmentReference(
		attachment_idx,//attachment_idx
		VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,//layout
		&p_vvo->color_attachment_reference//p_attachment_reference
	);

	return 1;
};

uint8_t vvoCreateDstImage(
	VvoHandle* p_vvo
) {
	vvoError(p_vvo == NULL, "vvoCreateDstImage: invalid vvo handle memory", return 0);

	shCreateImage(
		p_vvo->device,//device
		VK_IMAGE_TYPE_2D,//type
		p_vvo->surface_width,//x
		p_vvo->surface_height,//y
		1,//z
		p_vvo->src_image_format,//format
		1,//mip_levels
		1,//sample_count
		VK_IMAGE_TILING_LINEAR,//image_tiling
		VK_IMAGE_USAGE_TRANSFER_DST_BIT,//usage
		VK_SHARING_MODE_EXCLUSIVE,//sharing_mode
		&p_vvo->dst_image //p_image
	);

	shAllocateImageMemory(
		p_vvo->device,//device
		p_vvo->physical_device,//physical_device
		p_vvo->dst_image,//image
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,//memory_property_flags
		&p_vvo->dst_image_memory//p_image_memory
	);

	shBindImageMemory(
		p_vvo->device,//device
		p_vvo->dst_image,//image
		0,//offset
		p_vvo->dst_image_memory//image_memory
	);

	uint32_t stbi_row_size = p_vvo->src_image_channel_count * 1 * p_vvo->surface_width;//stbi expects 8 bits per channel
	p_vvo->p_stbi_image_data = calloc(1, p_vvo->surface_height * stbi_row_size);//stbi automatically allocates mem

	return 1;
}

uint8_t vvoCopyToDstImage(
	VvoHandle*       p_vvo,
	VkCommandBuffer  transfer_cmd_buffer,
	uint32_t         graphics_queue_family_index,
	uint32_t         transfer_queue_family_index,
	uint32_t         src_image_idx
) {
	vvoError(p_vvo               == NULL,           "vvoCopyToDstImage: invalid vvo handle memory",   return 0);
	vvoError(transfer_cmd_buffer == VK_NULL_HANDLE, "vvoCopyToDstImage: invalid transfer cmd buffer", return 0);

	shWaitForFences(
		p_vvo->device,//device
		1,//fence_count
		&p_vvo->image_copy_fence,//p_fences
		1,//wait_for_all
		UINT64_MAX//timeout_ns
	);

	shResetFences(
		p_vvo->device,//device
		1,//fence_count
		&p_vvo->image_copy_fence//p_fences
	);
	
	if (transfer_cmd_buffer != NULL) {
		shBeginCommandBuffer(transfer_cmd_buffer);
	}
	
	shSetImageMemoryBarrier(
		p_vvo->device,//device
		transfer_cmd_buffer,//cmd_buffer
		p_vvo->dst_image,//image
		VK_IMAGE_ASPECT_COLOR_BIT,//image_aspect_mask
		0,//access_before_barrier
		VK_ACCESS_TRANSFER_WRITE_BIT,//access_after_barrier
		VK_IMAGE_LAYOUT_UNDEFINED,//image_layout_before_barrier
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,//image_layout_after_barrier
		graphics_queue_family_index,//performing_queue_family_index_before_barrier
		transfer_queue_family_index,//performing_queue_family_index_after_barrier
		VK_PIPELINE_STAGE_TRANSFER_BIT,//pipeline_stage_before_barrier
		VK_PIPELINE_STAGE_TRANSFER_BIT//pipeline_stage_after_barrier
	);
	
	shCopyImage(
		transfer_cmd_buffer,//transfer_cmd_buffer
		p_vvo->surface_width,//width
		p_vvo->surface_height,//height
		VK_IMAGE_ASPECT_COLOR_BIT,//src_image_aspect
		VK_IMAGE_ASPECT_COLOR_BIT,//dst_image_aspect
		p_vvo->src_images[src_image_idx],//src_image
		p_vvo->dst_image//dst_image
	);
	
	if (transfer_cmd_buffer != NULL) {
		shEndCommandBuffer(transfer_cmd_buffer);
	}

	return 1;
}

uint8_t vvoSubmitImageCopy(
	VvoHandle* p_vvo,
	VkCommandBuffer  transfer_cmd_buffer,
	VkQueue          transfer_queue,
	VkSemaphore      graphics_queue_finished_semaphore
) {
	vvoError(p_vvo                             == NULL,           "vvoSubmitImageCopy: invalid vvo handle memory",   return 0);
	vvoError(transfer_cmd_buffer               == VK_NULL_HANDLE, "vvoSubmitImageCopy: invalid transfer cmd buffer", return 0);
	vvoError(transfer_queue                    == VK_NULL_HANDLE, "vvoSubmitImageCopy: invalid transfer queue",      return 0);
	vvoError(graphics_queue_finished_semaphore == VK_NULL_HANDLE, "vvoSubmitImageCopy: invalid semaphore",           return 0);

	VkCommandBuffer _transfer_cmd_buffer                = transfer_cmd_buffer;
	VkSemaphore     _graphics_queue_finished_semaphores = graphics_queue_finished_semaphore;

	shQueueSubmit(
		1,//cmd_buffer_count
		&_transfer_cmd_buffer,//p_cmd_buffers
		transfer_queue,//queue
		p_vvo->image_copy_fence,//fence
		1,//semaphores_to_wait_for_count
		&graphics_queue_finished_semaphore,//p_semaphores_to_wait_for
		VK_PIPELINE_STAGE_TRANSFER_BIT,//wait_stage --> onesto non so
		1,//signal_semaphore_count
		&p_vvo->image_copy_semaphore//p_signal_semaphores
	);

	p_vvo->image_submissions_count++;

	return 1;
}

uint8_t vvoFormatDstImageData(
	VvoHandle* p_vvo
) {
	vvoError(p_vvo == NULL, "vvoFormatDstImageData: invalid vvo handle memory", return 0);

	uint32_t vk_row_size   = p_vvo->src_image_channel_count * p_vvo->src_image_channel_size * p_vvo->surface_width;
	uint32_t stbi_row_size = p_vvo->src_image_channel_count * 1 * p_vvo->surface_width;//stbi expects 8 bits per channel
	
	//row_pitch (vulkan row size) might include also an offset between rows for alignment reasons
	uint32_t vk_row_pitch  = (uint32_t)p_vvo->dst_image_subresource_layout.rowPitch;
	uint32_t vk_row_offset = vk_row_pitch - vk_row_size;
	
	for (uint32_t row_idx = 0; row_idx < p_vvo->surface_height; row_idx++) {
	
		uint8_t* p_stbi_row = &p_vvo->p_stbi_image_data[stbi_row_size * row_idx];
		uint8_t* p_vk_row   = &p_vvo->p_dst_image_data [vk_row_pitch  * row_idx];
	
		for (uint32_t column_idx = 0; column_idx < p_vvo->surface_width; column_idx++) {
			
			uint8_t r8 = 0;
			uint8_t g8 = 0;
			uint8_t b8 = 0;
			uint8_t a8 = 0;
	
			uint16_t r16 = 0;
			uint16_t g16 = 0;
			uint16_t b16 = 0;
			uint16_t a16 = 0;
	
			uint32_t r32 = 0;
			uint32_t g32 = 0;
			uint32_t b32 = 0;
			uint32_t a32 = 0;
	
			uint64_t r64 = 0;
			uint64_t g64 = 0;
			uint64_t b64 = 0;
			uint64_t a64 = 0;
	
			switch (p_vvo->src_image_channel_size) {
	
			case 1://8 bits int
				r8 = ((uint8_t*)p_vk_row)[column_idx * p_vvo->src_image_channel_count + 0];
				g8 = ((uint8_t*)p_vk_row)[column_idx * p_vvo->src_image_channel_count + 1];
				b8 = ((uint8_t*)p_vk_row)[column_idx * p_vvo->src_image_channel_count + 2];
				a8 = ((uint8_t*)p_vk_row)[column_idx * p_vvo->src_image_channel_count + 3];
				break;
			
			case 2://16 bits int
				r16 = ((uint16_t*)p_vk_row)[column_idx * p_vvo->src_image_channel_count  + 0];
				g16 = ((uint16_t*)p_vk_row)[column_idx * p_vvo->src_image_channel_count  + 1];
				b16 = ((uint16_t*)p_vk_row)[column_idx * p_vvo->src_image_channel_count  + 2];
				a16 = ((uint16_t*)p_vk_row)[column_idx * p_vvo->src_image_channel_count  + 3];
				if (p_vvo->src_image_channel_type == SH_FORMAT_UINT) {
					r8 = (uint8_t)(r16 * UINT8_MAX / UINT16_MAX);
					g8 = (uint8_t)(g16 * UINT8_MAX / UINT16_MAX);
					b8 = (uint8_t)(b16 * UINT8_MAX / UINT16_MAX);
					a8 = (uint8_t)(a16 * UINT8_MAX / UINT16_MAX);
				}
				else if (p_vvo->src_image_channel_type == SH_FORMAT_SINT) {
					r8 = (uint8_t)(r16 * UINT8_MAX / INT16_MAX);
					g8 = (uint8_t)(g16 * UINT8_MAX / INT16_MAX);
					b8 = (uint8_t)(b16 * UINT8_MAX / INT16_MAX);
					a8 = (uint8_t)(a16 * UINT8_MAX / INT16_MAX);
				}
				break;
	
			case 4://32 bits 
				r32 = ((uint32_t*)p_vk_row)[column_idx * p_vvo->src_image_channel_count  + 0];
				g32 = ((uint32_t*)p_vk_row)[column_idx * p_vvo->src_image_channel_count  + 1];
				b32 = ((uint32_t*)p_vk_row)[column_idx * p_vvo->src_image_channel_count  + 2];
				a32 = ((uint32_t*)p_vk_row)[column_idx * p_vvo->src_image_channel_count  + 3];
				if (p_vvo->src_image_channel_type == SH_FORMAT_UINT) {
					r8 = (uint8_t)(r32 * UINT8_MAX / UINT32_MAX);
					g8 = (uint8_t)(g32 * UINT8_MAX / UINT32_MAX);
					b8 = (uint8_t)(b32 * UINT8_MAX / UINT32_MAX);
					a8 = (uint8_t)(a32 * UINT8_MAX / UINT32_MAX);
				}
				else if (p_vvo->src_image_channel_type == SH_FORMAT_SINT) {
					r8 = (uint8_t)(r32 * UINT8_MAX / INT32_MAX);
					g8 = (uint8_t)(g32 * UINT8_MAX / INT32_MAX);
					b8 = (uint8_t)(b32 * UINT8_MAX / INT32_MAX);
					a8 = (uint8_t)(a32 * UINT8_MAX / INT32_MAX);
				}
				break;
	
			case 8://64 bits
				r64 = ((uint64_t*)p_vk_row)[column_idx * p_vvo->src_image_channel_count  + 0];
				g64 = ((uint64_t*)p_vk_row)[column_idx * p_vvo->src_image_channel_count  + 1];
				b64 = ((uint64_t*)p_vk_row)[column_idx * p_vvo->src_image_channel_count  + 2];
				a64 = ((uint64_t*)p_vk_row)[column_idx * p_vvo->src_image_channel_count  + 3];
				if (p_vvo->src_image_channel_type == SH_FORMAT_UINT) {
					r8 = (uint8_t)(r64 * UINT8_MAX / UINT64_MAX);
					g8 = (uint8_t)(g64 * UINT8_MAX / UINT64_MAX);
					b8 = (uint8_t)(b64 * UINT8_MAX / UINT64_MAX);
					a8 = (uint8_t)(a64 * UINT8_MAX / UINT64_MAX);
				}
				else if (p_vvo->src_image_channel_type == SH_FORMAT_SINT) {
					r8 = (uint8_t)(r64 * UINT8_MAX / INT64_MAX);
					g8 = (uint8_t)(g64 * UINT8_MAX / INT64_MAX);
					b8 = (uint8_t)(b64 * UINT8_MAX / INT64_MAX);
					a8 = (uint8_t)(a64 * UINT8_MAX / INT64_MAX);
				}
				break;
			}
	
			(p_vvo->src_image_channel_count  >= 1) && (p_stbi_row[column_idx * p_vvo->src_image_channel_count  + 0] = r8);
			(p_vvo->src_image_channel_count  >= 2) && (p_stbi_row[column_idx * p_vvo->src_image_channel_count  + 1] = g8);
			(p_vvo->src_image_channel_count  >= 3) && (p_stbi_row[column_idx * p_vvo->src_image_channel_count  + 2] = b8);
			(p_vvo->src_image_channel_count  >= 4) && (p_stbi_row[column_idx * p_vvo->src_image_channel_count  + 3] = a8);
			
		}
	}

	return 1;
}

uint8_t vvoReadDstImage(
	VvoHandle* p_vvo
) {
	vvoError(p_vvo == NULL, "vvoReadDstImage: invalid vvo handle memory", return 0);

	shGetImageSubresourceLayout(
		p_vvo->device,
		p_vvo->dst_image,
		VK_IMAGE_ASPECT_COLOR_BIT,
		&p_vvo->dst_image_subresource_layout
	);

	p_vvo->dst_image_offset = (uint32_t)p_vvo->dst_image_subresource_layout.offset;

	shReadMemory(
		p_vvo->device,
		p_vvo->dst_image_memory,
		p_vvo->dst_image_offset,
		VK_WHOLE_SIZE,
		(void**)&p_vvo->p_dst_image_data,
		NULL
	);

	vvoFormatDstImageData(p_vvo);

	shUnmapMemory(
		p_vvo->device,//device
		p_vvo->dst_image_memory//memory
	);

	p_vvo->p_png_image_data = stbi_write_png_to_mem(
		p_vvo->p_stbi_image_data,//pixels
		p_vvo->surface_width * p_vvo->src_image_channel_count,//stride_bytes
		p_vvo->surface_width,//x
		p_vvo->surface_height,//y
		p_vvo->src_image_channel_count,//n
		&p_vvo->png_image_size//out_len
	);

	return 1;
}

uint8_t vvoGetDstImagePngData(
	VvoHandle* p_vvo
) {
	vvoError(p_vvo == NULL, "vvoGetDstImagePngData: invalid vvo handle memory", return 0);

	p_vvo->p_png_image_data = stbi_write_png_to_mem(
		p_vvo->p_stbi_image_data,//pixels
		p_vvo->surface_width * p_vvo->src_image_channel_count,//stride_bytes
		p_vvo->surface_width,//x
		p_vvo->surface_height,//y
		p_vvo->src_image_channel_count,//n
		&p_vvo->png_image_size//out_len
	);

	vvoError(
		p_vvo->p_png_image_data == NULL,
		"vvoGetDstImagePngData: failed saving png data",
		return 0
	);

	return 1;
}

uint8_t vvoWriteDstImageToDisk(
	VvoHandle* p_vvo,
	char*      path
) {
	vvoError(p_vvo == NULL, "vvoWriteDstImageToDisk: invalid vvo handle memory", return 0);
	vvoError(path  == NULL, "vvoWriteDstImageToDisk: invalid path",              return 0);

	stbi_write_png(
		"output.png",
		p_vvo->surface_width,
		p_vvo->surface_height,
		p_vvo->src_image_channel_count,
		p_vvo->p_stbi_image_data,
		p_vvo->surface_width * p_vvo->src_image_channel_count
	);

	return 1;
}

uint8_t vvoFreeStbiImageData(
	VvoHandle* p_vvo
) {
	vvoError(p_vvo == NULL, "vvoFreeDstImageData: invalid vvo handle memory", return 0);

	vvoError(
		p_vvo->p_stbi_image_data == NULL,
		"vvoFreDstImageData: invalid stbi image memory",
		return 0
	);

	free(p_vvo->p_stbi_image_data);

	return 1;
}

uint8_t vvoAddForwardSlashToUri(
	char* dst,
	char* src
) {
	vvoError(src == NULL, "vvoAddForwardSlashToUri: invalid src memory", return 0);
	vvoError(dst == NULL, "vvoAddForwardSlashToUri: invalid dst memory", return 0);

	if (src[0] != '/') { 
		memcpy(&dst[1], src, strlen(src));
		dst[0] = '/';
	}
	else {
		strcpy(dst, src);
	}

	return 1;
}

uint8_t vvoSetupServer(
	VvoHandle* p_vvo,
	char*      root_uri,
	char*      static_image_uri,
	char*      png_output_uri,
	char*      stream_uri
) {
	vvoError(p_vvo == NULL, "vvoPollEvents: invalid handle memory", return 0);

	vvoAddForwardSlashToUri(p_vvo->static_image_uri, static_image_uri);
	vvoAddForwardSlashToUri(p_vvo->png_output_uri,   png_output_uri);
	vvoAddForwardSlashToUri(p_vvo->stream_uri,       stream_uri);

	mg_mgr_init(&p_vvo->event_manager);

	mg_http_listen(&p_vvo->event_manager, root_uri, vvoHandleEvents, p_vvo);

	//p_connection->fn_data = p_vvo;

	return 1;
}

uint8_t vvoPollEvents(
	VvoHandle* p_vvo
) {
	vvoError(p_vvo == NULL, "vvoPollEvents: invalid handle memory", return 0);

	mg_mgr_poll(&p_vvo->event_manager, 1);

	if (p_vvo->p_png_image_data != NULL) {
		free(p_vvo->p_png_image_data);
	}

	return 1;
}

uint8_t vvoMainLoop(
	VvoHandle* p_vvo,
	uint8_t*   p_loop_condition
) {
	vvoError(p_vvo         == NULL, "vvoMainLoop: invalid handle memory",         return 0);
	vvoError(p_loop_condition == NULL, "vvoMainLoop: invalid loop condition memory", return 0);

	while (*p_loop_condition) {
		vvoPollEvents(p_vvo);
	}

	return 1;
}

uint8_t vvoVulkanRelease(
	VvoHandle* p_vvo
) {
	vvoError(p_vvo == NULL, "vvoVulkanRelease: invalid handle memory", return 0);

	VkDevice device = p_vvo->device;

	shWaitDeviceIdle(device);

	shDestroyImageViews(
		device, p_vvo->src_image_count, p_vvo->src_image_views
	);

	for (uint32_t src_image_idx = 0; src_image_idx < p_vvo->src_image_count; src_image_idx++) {
		shClearImageMemory(
			device,//device
			p_vvo->src_images[src_image_idx],//image
			p_vvo->src_images_memory[src_image_idx]//image_memory
		);
	}

	shClearImageMemory(
		device,//device
		p_vvo->dst_image,//image
		p_vvo->dst_image_memory//image_memory
	);

	shDestroySemaphores(device, 1, &p_vvo->image_copy_semaphore);

	shDestroyFences(device, 1, &p_vvo->image_copy_fence);

	return 1;
}

uint8_t vvoRelease(
	VvoHandle* p_vvo
) {
	vvoError(p_vvo == NULL, "vvoRelease: invalid handle memory", return 0);

	if (p_vvo->p_stbi_image_data != NULL) {
		free(p_vvo->p_stbi_image_data);
	}

	if (p_vvo->p_png_image_data != NULL) {
		p_vvo->png_image_size = 0;
		free(p_vvo->p_png_image_data);
	}

	mg_mgr_free(&p_vvo->event_manager);

	return 1;
}

void vvoHandleEvents(
	MongoConnection* p_connection,
	int              event, 
	void*            event_data
) {

	//vvoMessage(event == MG_EV_HTTP_MSG, "vvoHandleEvents: http connection established");
	//vvoMessage(event == MG_EV_WS_OPEN,  "vvoHandleEvents: websocket connection established");
	//vvoMessage(event == MG_EV_WS_MSG,   "vvoHandleEvents: websocket message received");

	VvoHandle*             p_vvo               = p_connection->fn_data;
	MongoHttpMessage*      p_http_message      = NULL;
	MongoWebSocketMessage* p_websocket_message = NULL;

	if (event == MG_EV_HTTP_MSG) {//http connections

		p_http_message = (MongoHttpMessage*)event_data;

		vvoError(p_http_message == NULL, "invalid http message memory", return);

		uint8_t time_request     = mg_match(p_http_message->uri, mg_str("/api/time/get"),          NULL);
		uint8_t square_request   = mg_match(p_http_message->uri, mg_str("/square"),                NULL);
		uint8_t squares_request  = mg_match(p_http_message->uri, mg_str("/squares"),               NULL);
		uint8_t image_request    = mg_match(p_http_message->uri, mg_str(p_vvo->static_image_uri),  NULL);
		uint8_t png_request      = mg_match(p_http_message->uri, mg_str(p_vvo->png_output_uri),    NULL);
		uint8_t stream_request   = mg_match(p_http_message->uri, mg_str(p_vvo->stream_uri),        NULL);
		uint8_t stream_websocket = mg_match(p_http_message->uri, mg_str("/vvoStreamWS"),           NULL);

		//html content requests
		     if (square_request )  { mg_http_reply(p_connection, VVO_HTTP_OK_INT, VVO_HTML_CONTENT_TYPE_HEADER, square);  }
		else if (squares_request)  { mg_http_reply(p_connection, VVO_HTTP_OK_INT, VVO_HTML_CONTENT_TYPE_HEADER, squares); }
		else if (image_request)    { mg_http_reply(p_connection, VVO_HTTP_OK_INT, VVO_HTML_CONTENT_TYPE_HEADER, image);   }
		else if (stream_request)   { mg_http_reply(p_connection, VVO_HTTP_OK_INT, VVO_HTML_CONTENT_TYPE_HEADER, web_socket_http_client); }

		//redirected requests 
		else if (stream_websocket) { mg_ws_upgrade(p_connection, p_http_message, NULL); } //change connection to websocket

		//assets requests
		else if (png_request) {

			mg_printf(p_connection, 
				VVO_HTTP_REPLY " " VVO_HTTP_OK
				VVO_PNG_CONTENT_TYPE_HEADER
				VVO_PNG_CONTENT_LENGTH_HEADER
				VVO_END_HTTP_REPLY
				,
				p_vvo->png_image_size
			);

			mg_send(p_connection, p_vvo->p_png_image_data, p_vvo->png_image_size);

		}
		else if (mg_match(p_http_message->uri, mg_str("/js/web_socket_http_client.js"), NULL)) {
			MongoHttpServeOptions opts = { .root_dir = NULL };
			mg_http_serve_file(p_connection, p_http_message, "js/web_socket_http_client.js", &opts);
		}
		else if (mg_match(p_http_message->uri, mg_str("/favicon.ico"), NULL)) {
		}

		else { 
			//printf("Unsupported URI: %s", p_http_message->uri.buf);
			mg_http_reply(p_connection, VVO_HTTP_NOT_FOUND_INT, "", "{%m:%m}\n", MG_ESC("error"), MG_ESC("Unsupported URI")); 
		}

	}

	if (event == MG_EV_WS_MSG) {//websocket connections
		p_websocket_message = (MongoWebSocketMessage*)event_data;
		vvoError(p_websocket_message == NULL, "invalid websocket message memory", return);
	}
	
	if (p_connection->is_websocket) {//while the connection is websocket
		mg_ws_send(p_connection, p_vvo->p_png_image_data, p_vvo->png_image_size, WEBSOCKET_OP_BINARY);
		//vvoSleepMs(1);
	}
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