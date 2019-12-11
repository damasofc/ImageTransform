
#define BOOST_NO_CXX11_SCOPED_ENUMS
#include <boost/filesystem.hpp>
#undef BOOST_NO_CXX11_SCOPED_ENUMS
#include <string>
#include <iostream>
#include <list>
#include <vector>
#include <Magick++.h>
#include <omp.h>
#include "timer.h"

using namespace std;

using namespace Magick;
using namespace boost::filesystem;
// ./main -s -f ./imagesTest/ ./imagesProccesed/

Image applyFilterGrayScale(Image &image);
Image applyFilterGrayScaleParallel(Image &image,int numT);

vector<vector<double>> getGaussianKernel(int w, int h,double sigma);
int main( int argc, char const *argv[])
{
  double start,finish;
  InitializeMagick(*argv);
  if(argc >= 3){
		string type = argv[1];
    string argumentTwo = argv[2];
    bool isFolder = argumentTwo.compare("-f") == 0;
    Image model;
    if (isFolder)
    {
      path p(argv[3]);
      directory_iterator itr(p);
      if(type.compare("-s") == 0){
        GET_TIME(start);
        for (auto i : itr)
        {
          string img = i.path().string();
          model = Image(img);
          cout<<argv[4]<<img.substr(2)<<endl;
          applyFilterGrayScale(model).write(argv[4]+img.substr(2));
        }
        GET_TIME(finish);
      }
      else
      {
        GET_TIME(start);
        int threads = stoi(argv[argc -1]);
        for (auto i : itr)
        {
          string img = i.path().string();
          model = Image(img);
          cout<<argv[4]<<img.substr(2)<<endl;
          applyFilterGrayScaleParallel(model,threads).write(argv[4]+img.substr(2));
        }
        GET_TIME(finish);
        cout<<"Num Threads: "<<threads<<endl;
      }
    }
    else
    {
      if(type.compare("-s") == 0){
        model = Image(argumentTwo);
        if(argc > 3){
          GET_TIME(start);
          applyFilterGrayScale(model).write(argv[4]);
          GET_TIME(finish);
        }else
        {
          GET_TIME(start);
          applyFilterGrayScale(model).write("newGrayScaleImage.jpeg");
          GET_TIME(finish);
        }
      }
      else
      {
        int threads = stoi(argv[argc -1]);
        model = Image(argumentTwo);
        if(argc > 3){
          GET_TIME(start);
          applyFilterGrayScaleParallel(model,2).write(argv[4]);
          GET_TIME(finish);
        }else
        {
          GET_TIME(start);
          applyFilterGrayScaleParallel(model,2).write("newGrayScaleImage.jpeg");
          GET_TIME(finish);
        }
        cout<<"\nNum Threads: "<<threads<<endl;
      }
       
    }
	}
  else
  {
    cout<<"You have to run the program like this examples: "<<endl;
    cout<<"\t0. ./main -s <image_path> "<<endl;
    cout<<"\t1. ./main -p <image_path> <# de threads> "<<endl;
    cout<<"\t2. ./main -s <image_path> -name <new_image_path> "<<endl;
    cout<<"\t3. ./main -p <image_path> -name <new_image_path> <# de threads> "<<endl;
    // cout<<"\t2. ./main -gaus <image_path> -name <new_image_path> "<<endl;
    cout<<"\t4. ./main -p -f <image_folders> <image_folder_destination> <# de threads> "<<endl;
    cout<<"\t4. ./main -s -f <image_folders> <image_folder_destination> "<<endl;
    // cout<<"\t4. ./main -gaus -f <image_folders> "<<endl;
    return 0;
  }

  cout<<"\nTiempo de ejecucion: "<<finish-start<<endl;

}

Image applyFilterGrayScale(Image &image)
{
  int imageH = image.rows();
  int imageW = image.columns();

  Image imgN(Geometry(imageW,imageH),"white");
  imgN.type(ImageType::TrueColorType);
  imgN.modifyImage();

  MagickCore::Quantum *pixels = imgN.getPixels(0,0,imageW,imageH);
  int i,j;
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

Image applyFilterGrayScaleParallel(Image &image,int numT)
{
  int imageH = image.rows();
  int imageW = image.columns();

  Image imgN(Geometry(imageW,imageH),"white");
  imgN.type(ImageType::TrueColorType);
  imgN.modifyImage();

  MagickCore::Quantum *pixels = imgN.getPixels(0,0,imageW,imageH);

  #pragma omp parallel for num_threads(numT) schedule(static,5)
  for (int i=0 ; i<imageH ; i++) {
    for (int j=0 ; j<imageW ; j++) {
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