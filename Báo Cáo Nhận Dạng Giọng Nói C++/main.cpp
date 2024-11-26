#include <raylib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <thread>

class Button {
public:
    Button(const char* imagePath, Vector2 imagePosition);
    ~Button();
    void Draw();
    bool isPressed(Vector2 mousePos, bool mousePressed);

private:
    Texture2D texture;
    Vector2 position;
};

Button::Button(const char* imagePath, Vector2 imagePosition) {
    texture = LoadTexture(imagePath);
    position = imagePosition;
}

Button::~Button() {
    UnloadTexture(texture);
}

void Button::Draw() {
    DrawTextureV(texture, position, GRAY);
}
//Dieu kien cho bien thanh vien isPressed
bool Button::isPressed(Vector2 mousePos, bool mousePressed) {
    Rectangle rect = {position.x, position.y, static_cast<float>(texture.width), static_cast<float>(texture.height)};
    return CheckCollisionPointRec(mousePos, rect) && mousePressed;
}

int main() {
    //Khoi tao khung chuong trinh
    InitWindow(760, 800, "Voice Recognition");
    SetTargetFPS(60);
    //Khoi tao cac nut
    Button startButton{"Graphics/microButton.png", {310, 600}};
    Button endButton{"Graphics/endButton.png", {560, 615}};
    Button textButton("Graphics/textButton.png", {100, 625});

    bool exit = false;
    bool showText = false;  // Biến điều khiển để hiển thị nội dung
    std::string content;    // Biến lưu trữ nội dung từ file

    while (!WindowShouldClose() && !exit) {
        Vector2 mousePosition = GetMousePosition();
        bool mousePressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
        //Dieu kien cua startButton
        if (startButton.isPressed(mousePosition, mousePressed)) {
            const char* filename = "stop_python.txt";
            if (remove(filename) == 0) {
                std::cout << "The voice is now recognizing" << std::endl;
            }
            std::thread pythonThread([]() {
                system("python voicerecognition.py");
            });
            pythonThread.detach();
        }
        //Dieu kien cua endButton
        if (endButton.isPressed(mousePosition, mousePressed)) {
            std::cout << "Stop listening" << std::endl;
            std::ofstream outfile("stop_python.txt");
            outfile.close();
            exit = true;
        }
        //Dieu kien cua textButton
        if (textButton.isPressed(mousePosition, mousePressed)) {
            // Chỉ đọc file một lần khi nhấn nút
            std::ifstream file("123.txt");
            if (file.is_open()) {
                content.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
                file.close();
                showText = true;  // Bật hiển thị nội dung sau khi đọc file
            } else {
                std::cout << "Unable to open file" << std::endl;
            }
        }
        // Bat dau ve
        BeginDrawing();
        ClearBackground(WHITE);
        startButton.Draw();
        endButton.Draw();
        textButton.Draw();
        // Kiểm tra và hiển thị nội dung chỉ khi showText được bật
        if (showText) {
            DrawText(content.c_str(), 100, 100, 30, BLACK);
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
