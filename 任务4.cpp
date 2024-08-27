#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
using namespace std;

class ImageMatrix {
private:
    vector<vector<int>> matrix;
    int width;
    int height;

public:
    // 构造函数
    ImageMatrix(int w = 0, int h = 0) {
        width = w;
        height = h;
        matrix.resize(h, vector<int>(w, 0));
    }

    // 获取图像的宽度
    int getWidth() const {
        return width;
    }

    // 获取图像的高度
    int getHeight() const {
        return height;
    }

    // 创建图像
    void createImage(int w, int h) {
        width = w;
        height = h;
        matrix.resize(height, vector<int>(width, 0));
    }

    // 判空
    bool isEmpty() const {
        return width == 0 || height == 0;
    }

    // 读取与保存
    void readFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Unable to open file: " + filename);
        }
        string line;
        vector<int> row;
        int value;
        // 读取文件中的每一行
        while (getline(file, line)) {
            row.clear(); // 清空当前行的像素值容器
            // 将字符串按照空白字符分割，读取每个像素值
            istringstream iss(line);
            while (iss >> value) {
                row.push_back(value);
            }
            // 如果是第一行，设置图像的高度和宽度
            if (matrix.empty()) {
                height = 1; // 至少有一行
                width = row.size();
                matrix.resize(height, vector<int>(width, 0));
            }
            // 确保每一行的像素值数量与宽度一致
            if (row.size() != width) {
                throw runtime_error("Inconsistent width in file: " + filename);
            }

            // 将当前行的像素值添加到矩阵中
            matrix.push_back(row);
            height++; // 更新高度
        }

        file.close();
    }

    void writeFile(const string& filename) const {
        ofstream file(filename);
        if (file.is_open()) {
            for (const auto& row : matrix) {
                for (int val : row) {
                    file << val << " ";
                }
                file << endl;
            }
            file.close();
        }
    }

    // 显示
    void display(int columnWidth = 4, int spacing = 3) const {
        for (const auto& row : matrix) {
            for (size_t j = 0; j < row.size(); ++j) {
                std::cout << setw(columnWidth) << left << row[j];
                // 如果不是最后一列，打印间距
                if (j + 1 < row.size()) {
                    cout << string(spacing, ' ');
                }
            }
            cout << endl;
        }
    }

    // 二值化显示
    void binaryDisplay() const {
        for (const auto& row : matrix) {
            for (int val : row) {
                cout << (val == 0 ? '.' : 'O');
            }
            cout << endl;
        }
    }

    // 绘制点
    void drawPoint(int x, int y, int value) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            matrix[y][x] = value;
        }
    }

    // 绘制框
    void drawBox(int x, int y, int w, int h, int value) {
        for (int i = y; i < min(y + h, height); ++i) {
            for (int j = x; j < min(x + w, width); ++j) {
                matrix[i][j] = value;
            }
        }
    }

    // 阈值化
    void threshold(int thr) {
        for (auto& row : matrix) {
            for (int& val : row) {
                val = val <= thr ? 0 : 255;
            }
        }
    }

    // 翻转
    void flipHorizontally() {
        for (auto& row : matrix) {
            for (int i = 0; i < width / 2; ++i) {
                swap(row[i], row[width - 1 - i]);
            }
        }
    }

    // 旋转
    void rotate90Degrees() {
        vector<vector<int>> newMatrix(width, vector<int>(height, 0));
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                newMatrix[j][height - 1 - i] = matrix[i][j];
            }
        }
        matrix = newMatrix;
        swap(width, height);
    }

    void drawEdge() {
        // 绘制顶部边缘（除了(0,0)点）
        for (int j = 1; j < width; ++j) {
            matrix[0][j] = 255;
        }
        // 绘制底部边缘
        for (int j = 0; j < width; ++j) {
            matrix[height - 1][j] = 255;
        }
        // 绘制左侧边缘（除了(0,0)点）
        for (int i = 1; i < height; ++i) {
            matrix[i][0] = 255;
        }
        // 绘制右侧边缘
        for (int i = 0; i < height; ++i) {
            matrix[i][width - 1] = 255;
        }
    }
};

int main() {
    string filePath = "D:\data.txt";
    ImageMatrix img;

    try {
        img.readFile(filePath);
        int thresholdValue = 101;
        img.threshold(thresholdValue);

        // 显示阈值化后的图像
        cout << "Image after thresholding:"<<endl;
        img.display(4, 3);

        // 执行翻转和旋转操作来摆正字符'R'
        img.flipHorizontally();
        for (int i = 0; i < 3; ++i) { 
            img.rotate90Degrees();
        }

        // 再次显示图像
        cout << endl<<"Image after flip and 270 degrees rotation to adjust 'R':"<<endl;
        img.display(4, 3);

        // 二值化显示
        cout <<endl<< "Binary display of the adjusted 'R':"<<endl;
        img.binaryDisplay();
    }
    catch (const runtime_error& e) {
        cerr << "Error reading file: " << e.what() << endl;
    }

    return 0;
}