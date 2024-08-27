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
    // ���캯��
    ImageMatrix(int w = 0, int h = 0) {
        width = w;
        height = h;
        matrix.resize(h, vector<int>(w, 0));
    }

    // ��ȡͼ��Ŀ��
    int getWidth() const {
        return width;
    }

    // ��ȡͼ��ĸ߶�
    int getHeight() const {
        return height;
    }

    // ����ͼ��
    void createImage(int w, int h) {
        width = w;
        height = h;
        matrix.resize(height, vector<int>(width, 0));
    }

    // �п�
    bool isEmpty() const {
        return width == 0 || height == 0;
    }

    // ��ȡ�뱣��
    void readFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Unable to open file: " + filename);
        }
        string line;
        vector<int> row;
        int value;
        // ��ȡ�ļ��е�ÿһ��
        while (getline(file, line)) {
            row.clear(); // ��յ�ǰ�е�����ֵ����
            // ���ַ������տհ��ַ��ָ��ȡÿ������ֵ
            istringstream iss(line);
            while (iss >> value) {
                row.push_back(value);
            }
            // ����ǵ�һ�У�����ͼ��ĸ߶ȺͿ��
            if (matrix.empty()) {
                height = 1; // ������һ��
                width = row.size();
                matrix.resize(height, vector<int>(width, 0));
            }
            // ȷ��ÿһ�е�����ֵ��������һ��
            if (row.size() != width) {
                throw runtime_error("Inconsistent width in file: " + filename);
            }

            // ����ǰ�е�����ֵ��ӵ�������
            matrix.push_back(row);
            height++; // ���¸߶�
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

    // ��ʾ
    void display(int columnWidth = 4, int spacing = 3) const {
        for (const auto& row : matrix) {
            for (size_t j = 0; j < row.size(); ++j) {
                std::cout << setw(columnWidth) << left << row[j];
                // ����������һ�У���ӡ���
                if (j + 1 < row.size()) {
                    cout << string(spacing, ' ');
                }
            }
            cout << endl;
        }
    }

    // ��ֵ����ʾ
    void binaryDisplay() const {
        for (const auto& row : matrix) {
            for (int val : row) {
                cout << (val == 0 ? '.' : 'O');
            }
            cout << endl;
        }
    }

    // ���Ƶ�
    void drawPoint(int x, int y, int value) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            matrix[y][x] = value;
        }
    }

    // ���ƿ�
    void drawBox(int x, int y, int w, int h, int value) {
        for (int i = y; i < min(y + h, height); ++i) {
            for (int j = x; j < min(x + w, width); ++j) {
                matrix[i][j] = value;
            }
        }
    }

    // ��ֵ��
    void threshold(int thr) {
        for (auto& row : matrix) {
            for (int& val : row) {
                val = val <= thr ? 0 : 255;
            }
        }
    }

    // ��ת
    void flipHorizontally() {
        for (auto& row : matrix) {
            for (int i = 0; i < width / 2; ++i) {
                swap(row[i], row[width - 1 - i]);
            }
        }
    }

    // ��ת
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
        // ���ƶ�����Ե������(0,0)�㣩
        for (int j = 1; j < width; ++j) {
            matrix[0][j] = 255;
        }
        // ���Ƶײ���Ե
        for (int j = 0; j < width; ++j) {
            matrix[height - 1][j] = 255;
        }
        // ��������Ե������(0,0)�㣩
        for (int i = 1; i < height; ++i) {
            matrix[i][0] = 255;
        }
        // �����Ҳ��Ե
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

        // ��ʾ��ֵ�����ͼ��
        cout << "Image after thresholding:"<<endl;
        img.display(4, 3);

        // ִ�з�ת����ת�����������ַ�'R'
        img.flipHorizontally();
        for (int i = 0; i < 3; ++i) { 
            img.rotate90Degrees();
        }

        // �ٴ���ʾͼ��
        cout << endl<<"Image after flip and 270 degrees rotation to adjust 'R':"<<endl;
        img.display(4, 3);

        // ��ֵ����ʾ
        cout <<endl<< "Binary display of the adjusted 'R':"<<endl;
        img.binaryDisplay();
    }
    catch (const runtime_error& e) {
        cerr << "Error reading file: " << e.what() << endl;
    }

    return 0;
}