#include <opencv2/opencv.hpp>
#include <iostream>

cv::Mat AgruparCinza(const cv::Mat&);
cv::Mat TransformarEmCinza(const cv::Mat &); 

int ExercicioUm();
int ExercicioDois();
int ExercicioTres();
int ExercicioQuatro();

int main(){
    ExercicioUm();
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
