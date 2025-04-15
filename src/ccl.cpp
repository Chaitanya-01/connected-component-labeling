#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

// Union Find Data Structure
class UnionFind{
    vector<int> parent;
public:
    UnionFind(int size){
        parent.resize(size);

        for (int i = 0; i<size; i++){
            parent[i] = i;
        }
    }

    int find(int i){
        // base case
        if (parent[i] == i){
            return i;
        }
        return find(parent[i]);
    }

    void unite(int i, int j){
        int irep = find(i);
        int jrep = find(j);

        parent[irep] = jrep; // ###########
    }

};

cv::Mat visualizeComponents(const cv::Mat& labels) {
        // Find the maximum label value
        double min_val, max_val;
        cv::minMaxLoc(labels, &min_val, &max_val);
        
        // Create a color image for visualization
        cv::Mat output = cv::Mat::zeros(labels.size(), CV_8UC3);
        
        // Generate random colors for each label
        std::vector<cv::Vec3b> colors(static_cast<int>(max_val) + 1);
        colors[0] = cv::Vec3b(0, 0, 0); // Background is black
        
        cv::RNG rng(12345);
        for (int i = 1; i <= static_cast<int>(max_val); i++) {
            colors[i] = cv::Vec3b(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
        }
        
        // Color each component
        for (int i = 0; i < output.rows; i++) {
            for (int j = 0; j < output.cols; j++) {
                int label = labels.at<int>(i, j);
                output.at<cv::Vec3b>(i, j) = colors[label];
            }
        }
        
        return output;
    }
// int getNeighbours(int i, int j, int row, int col){

// }



int main(int argc, char **argv){
    // Read the image in argv[1]
    cv::Mat image = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);
    // Check the data is correctly loaded.
    if (image.data == nullptr) {
        cerr << "file " << argv[1] << " do not exist" << endl;
        return 0;
    }
    // cout << "Image cols: " << image.cols << ", rows: " << image.rows << ", channels: " << image.channels() << endl;
    // Apply thresholding to convert to binary
    cv::Mat binaryImage;
    cv::threshold(image, binaryImage, 128, 255, cv::THRESH_BINARY);
    // cout << "Binary Image cols: " << binaryImage.cols << ", rows: " << binaryImage.rows << ", channels: " << binaryImage.channels() << endl;
    cout << binaryImage.size << endl; // image with 0 for all black pixels and 255 for white pixels

    // cv::imshow("binary image", binaryImage);
    // cv::waitKey(0);
    // cv::destroyAllWindows();

    UnionFind uf(binaryImage.cols*binaryImage.rows);
    // cout << uf.find(1) << endl;
    int connectivity = 8;


    // Algorithm
    // Initialize a 2D array the size of image with all elelemnts as bg label
    // int labels[binaryImage.rows][binaryImage.cols] = {0};
    // vector<int> parent;
    cv::Mat labels = cv::Mat::zeros(binaryImage.size(), CV_32SC1);
    int curr_label = 1;
    vector<int> neighbours;
    vector<int> neighbour_labels;
    for (int i = 0; i < binaryImage.rows; i++){
        for (int j = 0; j < binaryImage.cols; j++){
            if (binaryImage.at<uchar>(i, j) != 0){
                // Check on neighbours
                if (connectivity == 8){
                    neighbours = {binaryImage.at<uchar>(i-1, j-1), binaryImage.at<uchar>(i-1, j), binaryImage.at<uchar>(i-1, j+1), binaryImage.at<uchar>(i, j-1)};
                }
                
                if (std::find(neighbours.begin(), neighbours.end(), 255) == neighbours.end()){
                    
                    labels.at<int>(i,j) = curr_label;
                    curr_label++;
                }
                else {

                    if (connectivity == 8){
                        neighbour_labels = {labels.at<int>(i-1, j-1), labels.at<int>(i-1, j), labels.at<int>(i-1, j+1), labels.at<int>(i, j-1)};
                    }
                    int min_val;
                    if (std::find(neighbour_labels.begin(), neighbour_labels.end(), 0) != neighbours.end()){
                        min_val = *std::max_element(neighbour_labels.begin(), neighbour_labels.end());
                        for (const auto& element : neighbour_labels){
                            if (element != 0 && element < min_val){
                                min_val = element;
                            }
                        }
                    } else{
                        min_val = *std::min_element(neighbour_labels.begin(), neighbour_labels.end());
                    }
                    labels.at<int>(i, j) = min_val;
                    for (const auto& element : neighbour_labels){
                        if (element != 0){
                            uf.unite(element, min_val);
                        }
                    }

                }

                

            }

        }
    }

    //second pass
    for (int i = 0; i < binaryImage.rows; i++){
        for (int j = 0; j < binaryImage.cols; j++){
            if (binaryImage.at<uchar>(i, j) != 0){
                int root = uf.find(labels.at<int>(i,j));
                labels.at<int>(i,j) = root;
            }

        }
    }


    cv::Mat labels_out = visualizeComponents(labels);
    cv::imshow("image", labels_out);
    // cv::imshow("binary image", binaryImage);
    cv::waitKey(0);
    cv::destroyAllWindows();
    return 0;
}