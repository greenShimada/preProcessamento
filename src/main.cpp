#include <opencv2/opencv.hpp>
#include <iostream>

cv::Mat AgruparCinza(const cv::Mat&);
cv::Mat TransformarEmCinza(const cv::Mat &); 
cv::Mat SubtrairImagens(const cv::Mat &, const cv::Mat &);
cv::Mat BinarizarImagem(const cv::Mat &);
int ExercicioUm();
int ExercicioDois();
int ExercicioTres();
int ExercicioQuatro();
cv::Mat HighBoostFilter(const cv::Mat & , float );

#define IMG01_1         "../imgs/color_img.jpg"

#define IMG02_COM01     "../imgs/com01.jpeg"
#define IMG02_SEM01     "../imgs/sem01.jpeg"

//#define IMG02_COM02     "../imgs/com02.jpeg"
//#define IMG02_SEM02     "../imgs/sem02.jpeg"

using namespace cv;
int main(){
        
    // ExercicioUm();
    //ExercicioDois();
    ExercicioTres();
}
int ExercicioTres() {
    cv::Mat image = cv::imread(IMG02_COM01);
    if (image.empty()) {
        std::cout << "Erro ao carregar imagem!" << std::endl;
        return -1;
    }
    cv::Mat resized;
    cv::resize(image, resized, cv::Size(600, 600));

    // Converte para tons de cinza
    cv::Mat gray;
    gray = TransformarEmCinza(resized);

    cv::Mat highpass;
    cv::Laplacian(gray, highpass, CV_16S, 3);
    cv::convertScaleAbs(highpass, highpass);

    cv::Mat highBoost = HighBoostFilter(gray, 10.5f);


    cv::imshow("Original", resized);
    cv::imshow("Cinza", gray);
    cv::imshow("Filtro Passa-Alta", highpass);
    cv::imshow("Filtro High-Boost 10.5", highBoost);
    cv::waitKey(0);

    return 0;
}


int ExercicioDois(){

    cv::Mat imagem_com01 = cv::imread(IMG02_COM01);
    cv::Mat imagem_sem01 = cv::imread(IMG02_SEM01);
	cv::Mat result;
    cv::Mat binary;

    if ( imagem_com01.empty() || imagem_sem01.empty() ) {
        std::cout << "Erro ao carregar a imagem!" << std::endl;
        return -1;
    }
    cv::Mat redimCom;
    cv::Mat redimSem;

    cv::resize(imagem_com01, redimCom, cv::Size(600, 600));
    cv::resize(imagem_sem01, redimSem, cv::Size(600, 600));

    result = SubtrairImagens(redimSem, redimCom);  
    binary = BinarizarImagem(result);

    cv::imshow("Imagem subtraida", result);
    cv::imshow("Imagem binarizada", binary);
    cv::waitKey(0);
}

int ExercicioUm() {
    cv::Mat imagem = cv::imread("../imgs/color_img.jpg");
	
    if (imagem.empty()) {
        std::cout << "Erro ao carregar a imagem!" << std::endl;
        return -1;
    }
	
    cv::Mat redim; // pra ficar igual x e y e facilitar as contas
    cv::resize(imagem, redim, cv::Size(600, 600));
    cv::imshow("Imagem original", redim);

    cv::Mat cinza = TransformarEmCinza(redim);
    cv::imshow("Imagem cinza", cinza);

    cv::Mat agrupada = AgruparCinza(cinza);
    cv::imshow("Imagem agrupada", agrupada);

    cv::waitKey(0);
    return 0;
}

cv::Mat BinarizarImagem(const cv::Mat &image) {
    int cols = image.cols;
    int rows = image.rows;

    cv::Mat binaryImage = image.clone();

    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            cv::Vec3b pixel = image.at<cv::Vec3b>(y, x);
            int soma = static_cast<int>(pixel[0]) + pixel[1] + pixel[2]; 

            if (soma < 130) {
                binaryImage.at<cv::Vec3b>(y, x) = cv::Vec3b(0, 0, 0); 
            } else {
                binaryImage.at<cv::Vec3b>(y, x) = cv::Vec3b(255, 255, 255); 
            }
        }
    }

    return binaryImage;
}


cv::Mat SubtrairImagens(const cv::Mat &imageWith, const cv::Mat &imageWithout){
    
    int colsWith, colsWithout, rowsWith, rowsWithout;

    colsWith = imageWith.cols;
    colsWithout = imageWithout.cols;
    rowsWith = imageWith.rows;
    rowsWithout = imageWithout.rows;
    cv::Mat result = imageWithout.clone();


    if (colsWith != colsWithout || rowsWith != rowsWithout){
        std::cout << "Erro ao subtrair imagens! Imagens de tamanhos diferentes!" << std::endl;
        return result;
    }

    std::cout << "Colunas: " << colsWith << std::endl;
    std::cout << "Linhas: " << rowsWith << std::endl;

    for (int y = 0; y < rowsWith; ++y) {
        for (int x = 0; x < colsWith; ++x) {
            cv::Vec3b pixelWithout = imageWithout.at<cv::Vec3b>(y, x);
            cv::Vec3b pixelWith = imageWith.at<cv::Vec3b>(y, x);

            cv::Vec3b pixelResult;

            for (int c = 0; c < 3; ++c) {
                int diff = pixelWithout[c] - pixelWith[c];
                pixelResult[c] = cv::saturate_cast<uchar>(std::abs(diff)); 
            }

            result.at<cv::Vec3b>(y, x) = pixelResult;
        }
    }

    return result;
}

cv::Mat TransformarEmCinza(const cv::Mat &image){
    int rows = image.rows;
    int cols = image.cols;

    cv::Mat gray_img(rows, cols, CV_8UC1);

    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            cv::Vec3b pixel = image.at<cv::Vec3b>(y, x);
            uchar B = pixel[0];
            uchar G = pixel[1];
            uchar R = pixel[2];

            uchar tonsDeCinza = static_cast<uchar>((R+G+B)/3);

            gray_img.at<uchar>(y, x) = tonsDeCinza;
        }
    }
    return gray_img;
}

cv::Mat AgruparCinza(const cv::Mat &gray_img) {
    int rows = gray_img.rows;
    int cols = gray_img.cols;

    cv::Mat resultado = gray_img.clone();
    for (int y = 0; y < rows - 1; y += 2) {
        for (int x = 0; x < cols - 1; x += 2) {
            int soma = gray_img.at<uchar>(y, x) +
                       gray_img.at<uchar>(y, x + 1) +
                       gray_img.at<uchar>(y + 1, x) +
                       gray_img.at<uchar>(y + 1, x + 1);

            uchar media = static_cast<uchar>(soma / 4);

            resultado.at<uchar>(y, x) = media;
            resultado.at<uchar>(y, x + 1) = media;
            resultado.at<uchar>(y + 1, x) = media;
            resultado.at<uchar>(y + 1, x + 1) = media;
        }
    }

    return resultado;
}


cv::Mat HighBoostFilter(const Mat& image, float A) {
    Mat blurred, mask, highBoost;

    GaussianBlur(image, blurred, Size(5, 5), 0);

    subtract(image, blurred, mask);

    highBoost = image + (A - 1) * mask;

    return highBoost;
}

