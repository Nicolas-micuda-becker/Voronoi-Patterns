# Voronoi-Patterns

**Author:** Nicolas Micuda  
**Project Type:** Individual  

## Description  
This project transforms an input image into a **Voronoi-based** version using circles.  
The algorithm partitions the image into regions based on pixel colors, creating a visual effect inspired by Voronoi diagrams.  
The rendering is handled using **GL4D**, while **SDL2** is used for window and BMP image management.  

## Acknowledgments  
This project was made possible thanks to the support of **Farès Belhadj**, my professor and the creator of **GL4D**.  
He provided valuable assistance, particularly in refining the **Makefile**,the **tiny4D.h** and the **Documentation**.

## Instructions  

### Compilation  
```bash
make
```

### Execution  

#### Running the Program  
```bash
./circles <input_image.bmp>
```
Example:  
```bash
./circles test.bmp
```

### Cleaning Build Files  
To remove the files generated during compilation, use:  
```bash
make clean
```

## Features  

### Image Processing  
- Loads a BMP image and initializes a rendering surface.  
- Selects random seed points to generate circular regions.  
- Assigns colors to each region based on the initial pixel.  
- Displays the final image in an OpenGL window.  

## Dependencies  
- **GL4D**: Handles OpenGL rendering.  
- **SDL2**: Manages window creation and BMP image loading.  

Ensure these libraries are installed before compiling the project.  

## Known Issues  
- The image is currently flipped during processing. A fix is in progress.  

### Future Enhancements  
- Correct the image orientation.  
- Add support for more image formats beyond BMP.  

## License  
This project is distributed under the **GNU GPL** license. Refer to the `LICENSE` file for more details.  
