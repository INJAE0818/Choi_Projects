#include "FontLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cctype>

// 폴백: 정규화 좌표(0..1)에서 테두리 사각형
FontCharacter FontLoader::makeFallbackRect() {
    FontCharacter fc;
    FontContour c;
    const float m = 0.15f;
    c.points.push_back({ m, m });
    c.points.push_back({ 1.0f - m, m });
    c.points.push_back({ 1.0f - m, 1.0f - m });
    c.points.push_back({ m, 1.0f - m });
    fc.contours.push_back(std::move(c));
    return fc;
}

char FontLoader::indexToChar(int code) {
    if ((code >= '0' && code <= '9') ||
        (code >= 'A' && code <= 'Z') ||
        (code >= 'a' && code <= 'z')) return (char)code;
    return 0;
}

bool FontLoader::loadFont(const std::string& path)
{
    table_.clear();

    std::ifstream fin(path);
    if (!fin.is_open()) {
        std::cerr << "Failed to open font file: " << path << " (using fallback glyphs)\n";
        for (char ch = '0'; ch <= '9'; ++ch) table_[ch] = makeFallbackRect();
        for (char ch = 'A'; ch <= 'Z'; ++ch) table_[ch] = makeFallbackRect();
        for (char ch = 'a'; ch <= 'z'; ++ch) table_[ch] = makeFallbackRect();
        return false;
    }

    std::string line;
    std::string tok;
    FontCharacter current;
    FontContour   contour;
    bool inContour = false;

    auto flushContour = [&]() {
        if (!contour.points.empty()) current.addContour(contour);
        contour = FontContour{};
        inContour = false;
        };
    auto flushChar = [&]() {
        if (current.ch != 0) {
            if (inContour) flushContour();
            current.normalize();
            table_[current.ch] = current;
        }
        current = FontCharacter{};
        };

    // 매우 유연한 파서: 키워드만 대략 인식하고 좌표는 x y 페어로 저장
    while (std::getline(fin, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) continue;

        std::istringstream iss(line);
        iss >> tok;
        if (tok.empty()) continue;

        if (tok == "Index") {            // 새 글자 시작
            flushChar();
        }
        else if (tok == "CharIndex") {   // 코드값
            int code = 0; iss >> code;
            current.ch = indexToChar(code);
        }
        else if (tok == "BBox") {        // xmin ymin xmax ymax
            iss >> current.bbox[0] >> current.bbox[1] >> current.bbox[2] >> current.bbox[3];
        }
        else if (tok == "Contour") {     // 윤곽 시작
            if (inContour) flushContour();
            int idx = 0, n = 0; iss >> idx >> n; // n은 참고만
            inContour = true;
        }
        else if ((tok == "L" || tok == "C") && inContour) { // 좌표들
            std::vector<float> vals;
            float v;
            while (iss >> v) vals.push_back(v);
            for (size_t i = 0; i + 1 < vals.size(); i += 2)
                contour.points.emplace_back(vals[i], vals[i + 1]);
        }
        else {
            // 좌표만 있는 줄일 수도 있음 → 수치로 파싱 시도
            std::vector<float> vals;
            // 첫 토큰도 수로 시도
            char* endp = nullptr;
            float first = std::strtof(tok.c_str(), &endp);
            if (endp && *endp == '\0') vals.push_back(first);
            float v;
            while (iss >> v) vals.push_back(v);
            if (inContour && vals.size() >= 2) {
                for (size_t i = 0; i + 1 < vals.size(); i += 2)
                    contour.points.emplace_back(vals[i], vals[i + 1]);
            }
        }
    }

    // 마지막 글자 마감
    if (current.ch != 0) {
        if (inContour) flushContour();
        current.normalize();
        table_[current.ch] = current;
    }

    // 테이블이 비면 폴백
    if (table_.empty()) {
        for (char ch = '0'; ch <= '9'; ++ch) table_[ch] = makeFallbackRect();
        for (char ch = 'A'; ch <= 'Z'; ++ch) table_[ch] = makeFallbackRect();
        for (char ch = 'a'; ch <= 'z'; ++ch) table_[ch] = makeFallbackRect();
    }

    std::cout << "Loaded " << table_.size() << " characters from " << path << "\n";
    return true;
}

const FontCharacter* FontLoader::getCharacter(char ch) const
{
    auto it = table_.find(ch);
    return (it == table_.end()) ? nullptr : &it->second;
}
