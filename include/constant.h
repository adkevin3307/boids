#pragma once

namespace CONSTANT {
    enum BUTTON {
        LEFT,
        RIGHT
    };

    enum TRANSFORMATION {
        MODEL,
        VIEW,
        PROJECTION,
        MODEL_VIEW_PROJECTION,
        TRANSLATE,
        SCALE,
        ROTATE
    };

    const static int WIDTH = 1440;
    const static int HEIGHT = 900;
    const static int DEPTH = 5000;
}; // namespace CONSTANT
