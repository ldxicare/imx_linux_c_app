source /opt/fsl-imx-x11/4.1.15-2.1.0/environment-setup-cortexa7hf-neon-poky-linux-gnueabi
${CC} -o freetype_test freetype_test.c -I/home/lai/linux/download/tool/freetype/include/freetype2 -L/home/lai/linux/download/tool/freetype/lib -lfreetype -L/home/lai/linux/download/tool/zlib/lib -lz -L/home/lai/linux/download/tool/libpng/lib -lpng -lm
