#pragma once
#include <vector>
#include <map>
#include <string>
#include <glm/glm.hpp>

// 한 개 외곽선(윤곽)
struct FontContour {
    std::vector<glm::vec2> points;
    void addPoint(float x, float y) { points.emplace_back(x, y); }
};

// 한 글자
struct FontCharacter {
    char ch = 0;
    std::vector<FontContour> contours;
    int bbox[4] = { 0, 0, 0, 0 }; // xmin, ymin, xmax, ymax

    FontCharacter() = default;
    explicit FontCharacter(char c) : ch(c) {}

    void addContour(const FontContour& c) { contours.push_back(c); }

    int getTotalPoints() const {
        int s = 0;
        for (const auto& c : contours) s += (int)c.points.size();
        return s;
    }

    void normalize() {
        if (bbox[2] == bbox[0] || bbox[3] == bbox[1]) return;
        float w = (float)(bbox[2] - bbox[0]);
        float h = (float)(bbox[3] - bbox[1]);
        for (auto& c : contours) {
            for (auto& p : c.points) {
                p.x = (p.x - (float)bbox[0]) / w;
                p.y = (p.y - (float)bbox[1]) / h;
            }
        }
    }
};

class FontLoader {
public:
    FontLoader() = default;
    bool loadFont(const std::string& path);              // 파일에서 로드 (실패해도 fallback 채움)
    const FontCharacter* getCharacter(char ch) const;    // 없으면 nullptr

private:
    std::map<char, FontCharacter> table_;
    static FontCharacter makeFallbackRect();             // 파싱 실패 시 사용할 기본 글리프
    static char indexToChar(int code);                   // 48..57, 65..90, 97..122
};
