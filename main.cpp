
#include <string>
#include <iostream>
#include <list>
#include <vector>
#include <Magick++.h>

using namespace std;

using namespace Magick;

Image applyFilter(Image &image, vector<vector<double>> kernel);
Image applyFilterGrayScale(Image &image);
int main( int argc, char const *argv[])
{

  // Initialize ImageMagick install location for Windows
  InitializeMagick(*argv);
  // cout << "Read images ..." << endl;
  vector<vector<double>> kernel{
    {0,-1,0},
    {-1,5,-1},
    {0,-1,0}
    };
  
  Image model("../imagesTest/plotly.png" );
  applyFilterGrayScale(model);

  // cout<< model.pixelColor(50,50).quantumRed()<<endl;

  // cout<<model.size().width()<<endl;
}

Image applyFilterGaussian(Image &image, vector<vector<double>> kernel)
{
  int imageH = image.rows();
  int imageW = image.columns();
  int kernelH = kernel.size();
  int kernelW = kernel[0].size();

  Image imgN(Geometry(imageW,imageH),"white");
  imgN.type(ImageType::TrueColorType);
  imgN.modifyImage();

  MagickCore::Quantum *pixels = imgN.getPixels(0,0,imageW,imageH);
  int i,j,h,w;
  for (i=0 ; i<imageW ; i++) {
    for (j=0 ; j<imageH ; j++) {
      for (h=i ; h<i+kernelW ; h++) {
        for (w=j ; w<j+kernelH ; w++) { 
          unsigned offset = imgN.channels() * (imageW * j + i);
          pixels[offset + 0] = kernel[h-i][w-j] * image.pixelColor(i,j).quantumRed();
          pixels[offset + 1] = kernel[h-i][w-j] * image.pixelColor(i,j).quantumGreen();
          pixels[offset + 2] = kernel[h-i][w-j] * image.pixelColor(i,j).quantumBlue();
            // newImage[d][i][j] += filter[h-i][w-j]*image[d][h][w];
        }
      }
    }
  }

  imgN.syncPixels();

  imgN.write("pruebaImagen.jpeg");
  return imgN;
}

Image applyFilterGrayScale(Image &image)
{
  int imageH = image.rows();
  int imageW = image.columns();

  Image imgN(Geometry(imageW,imageH),"white");
  imgN.type(ImageType::TrueColorType);
  imgN.modifyImage();

  MagickCore::Quantum *pixels = imgN.getPixels(0,0,imageW,imageH);
  int i,j,h,w;
  for (i=0 ; i<imageH ; i++) {
    for (j=0 ; j<imageW ; j++) {
      unsigned offset = imgN.channels() * (imageW * i + j);
        double r = image.pixelColor(j,i).quantumRed()*0.2126;
        double g = image.pixelColor(j,i).quantumGreen()*0.7152;
        double b = image.pixelColor(j,i).quantumBlue()*0.0722;
        double res = r+g+b;
        pixels[offset + 0] = res;
        pixels[offset + 1] = res;
        pixels[offset + 2] = res;
    }
  }

  imgN.syncPixels();

  imgN.write("../grayscale.jpeg");
  return imgN;
}