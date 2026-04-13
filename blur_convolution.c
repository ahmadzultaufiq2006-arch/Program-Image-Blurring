#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

void apply_blur(unsigned char* in, unsigned char* out, int w, int h, int c, int k_size) {
    int r = k_size / 2;
    
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            for (int ch = 0; ch < c; ch++) {
                float sum = 0;
                int count = 0;
                
                for (int ky = -r; ky <= r; ky++) {
                    for (int kx = -r; kx <= r; kx++) {
                        int py = y + ky;
                        int px = x + kx;
                        
                        if (px >= 0 && px < w && py >= 0 && py < h) {
                            sum += in[(py * w + px) * c + ch];
                            count++;
                        }
                    }
                }
                
                out[(y * w + x) * c + ch] = (unsigned char)(sum / count);
            }
        }
    }
}

int main() {
    int width, height, channels;
    
    unsigned char *img_in = stbi_load("Kupu-kupu.png", &width, &height, &channels, 0);
    
    if (img_in == NULL) {
        printf("Error: Gagal memuat 'Kupu-kupu.png'. Pastikan file ada di folder yang sama.\n");
        return 1;
    }
    
    printf("Gambar berhasil dimuat! Lebar: %d, Tinggi: %d, Channels: %d\n", width, height, channels);

    size_t img_size = width * height * channels;
    unsigned char *img_out = (unsigned char *)malloc(img_size);
    
    if (img_out == NULL) {
        printf("Error: Gagal mengalokasikan memori untuk gambar output.\n");
        stbi_image_free(img_in);
        return 1;
    }

    int kernel_size = 7; 
    printf("Menerapkan blur dengan ukuran kernel %dx%d...\n", kernel_size, kernel_size);
    apply_blur(img_in, img_out, width, height, channels, kernel_size);

    int stride_in_bytes = width * channels;
    if (stbi_write_png("Kupu-kupu_blur.png", width, height, channels, img_out, stride_in_bytes)) {
        printf("Berhasil! Gambar telah disimpan sebagai 'Kupu-kupu_blur.png'\n");
    } else {
        printf("Error: Gagal menyimpan gambar.\n");
    }

    stbi_image_free(img_in);
    free(img_out);

    return 0;
}