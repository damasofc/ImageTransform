
#include <string>
#include <iostream>
#include <list>
#include <vector>
#include <Magick++.h>

using namespace std;

using namespace Magick;

Image applyFilterGaussian(Image &image, vector<vector<double>> kernel);

Image applyFilterGrayScale(Image &image);

vector<vector<double>> getGaussianKernel(int w, int h,double sigma);
int main( int argc, char const *argv[])
{

  InitializeMagick(*argv);
  if(argc >= 3){
		string type = argv[1];
    Image model;
    if(type.compare("-gray") == 0){
      model = Image(argv[2]);
      if(argc > 3){
        applyFilterGrayScale(model).write(argv[4]);
      }else
      {
        applyFilterGrayScale(model).write("newGrayScaleImage.jpeg");
      }
    }
    else if(type.compare("-gaus") == 0){
      vector<vector<double>> kernel = getGaussianKernel(5,5,10.0);
      model = Image(argv[2]);
      if(argc > 3){
        applyFilterGaussian(model,kernel).write(argv[4]);
      }else
      {
        applyFilterGaussian(model,kernel).write("newGrayScaleImage.jpeg");
      }
    }
	}
  else
  {
    cout<<"You have to run the program like this examples: "<<endl;
    cout<<"\t1. ./main -gray <image_path> -name <new_image_path> "<<endl;
    cout<<"\t2. ./main -gaus <image_path> -name <new_image_path> "<<endl;
    cout<<"\t3. ./main -gray -f <image_folders> "<<endl;
    cout<<"\t4. ./main -gaus -f <image_folders> "<<endl;
    return 0;
  }

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
  for (i=0 ; i<imageH ; i++) {
    for (j=0 ; j<imageW ; j++) {
      for (h=i ; h<i+kernelH ; h++) {
        for (w=j ; w<j+kernelW ; w++) {
          unsigned offset = imgN.channels() * (imageW * i + j);
          pixels[offset + 0] = kernel[h-i][w-j] * image.pixelColor(h,w).quantumRed();
          pixels[offset + 1] = kernel[h-i][w-j] * image.pixelColor(h,w).quantumGreen();
          pixels[offset + 2] = kernel[h-i][w-j] * image.pixelColor(h,w).quantumBlue();
        }
      }
    }
  }

  imgN.syncPixels();
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
  return imgN;
}

vector<vector<double>> getGaussianKernel(int w, int h,double sigma)
{
  vector<vector<double>> res = vector<vector<double>>();
  double sum = 0.0;
  for (size_t i = 0; i < h; i++)
  {
    res.push_back(vector<double>());
    for (size_t j = 0; j < w; j++)
    {
      double gaussianVal = exp(-(i*i+j*j)/(2*sigma*sigma))/(2*M_PI*sigma*sigma);
      res[i].push_back(gaussianVal);
      sum += gaussianVal;
    }
    
  }

  for (size_t i = 0; i < h; i++)
  {
    for (size_t j = 0; j < w; j++)
    {
      res[i][j] /= sum;
    }
    
  }
  return res;
  
}