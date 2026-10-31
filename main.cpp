#include <raylib.h>
constexpr int window_width{1800};
constexpr int window_height{1000};
constexpr int max_v_size{5000};
constexpr float threshold{15};
constexpr float threshold_sqrd{threshold*threshold};
float noise{25};


struct ParticleStruct{
    public:
    float x[max_v_size];
    float y[max_v_size];
    float velocity_x[max_v_size];
    float velocity_y[max_v_size];
    bool isfrozen[max_v_size];
    Color color[max_v_size];
    float parent_x[max_v_size];
    float parent_y[max_v_size];
    float radius[max_v_size];
    int hue[max_v_size];


};

ParticleStruct particles;

void Reset(){
    for(int i{1}; i < max_v_size; i++){
        particles.x[i] =GetRandomValue(0, window_width);
        particles.y[i] =GetRandomValue(0, window_height);
        particles.radius[i] = 5;
        particles.velocity_x[i] =GetRandomValue(6, 14);
        particles.velocity_y[i] =GetRandomValue(6, 14);
        particles.isfrozen[i] = false;
        particles.parent_x[i] = 0.0;
        particles.parent_y[i] = 0.0;
        particles.color[i] = GRAY;
        particles.hue[i] = 0;

    }

    particles.x[0] = window_width/2;
    particles.y[0] = window_height;
    particles.isfrozen[0] = true;
    particles.color[0] = RED;
    particles.hue[0] = 1;
    particles.radius[0] = 5; 
    

}


int main(){
    InitWindow(window_width, window_height, "dla");
    SetTargetFPS(60);
    
    Rectangle sliderBounds{5,window_height-105,250,100};
    bool isdragging = false;
    float& sliderValue = noise;

    Reset();


    
    while(!WindowShouldClose()){
        BeginDrawing();

        Vector2 mousePos = GetMousePosition();
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            if (CheckCollisionPointRec(mousePos, sliderBounds)) {
                isdragging = true;
            }
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            isdragging = false;
        }
        
        if (isdragging) {
       
            float normalizedX = (mousePos.x - sliderBounds.x) / sliderBounds.width;
            
           
            if (normalizedX < 0.0f) normalizedX = 0.0f;
            if (normalizedX > 1.0f) normalizedX = 1.0f;

            
            sliderValue = normalizedX * 100.0f;
        }


        for(int i{1}; i < max_v_size; i++){
            if(!particles.isfrozen[i]){
            particles.x[i] += GetRandomValue(-noise, noise);
            particles.y[i] += GetRandomValue(-noise, noise) + 1;
            particles.x[i] = static_cast<float>(static_cast<int>(particles.x[i] + window_width) % window_width);
            if(particles.y[i] > window_height){
                particles.y[i] = 1;
                particles.x[i] = GetRandomValue(5, window_width-5);
            }
        }
        }

        for(int i{0}; i < max_v_size; ++i){
            if(!particles.isfrozen[i]){
            for(int j{0}; j < max_v_size ; ++j ){
                if(particles.isfrozen[j] && j != i){
                    float dx = particles.x[i] - particles.x[j];
                    float dy = particles.y[i] - particles.y[j];
                    float distance_sqrd = dx * dx + dy * dy;
                    if(distance_sqrd < threshold_sqrd){
                        particles.parent_x[i] = particles.x[j];
                        particles.parent_y[i] = particles.y[j];
                        particles.isfrozen[i] = true;
                        particles.hue[i] = (particles.hue[j] + 4) % 360;
                        particles.color[i] = ColorFromHSV(particles.hue[i], 0.75, 1);
                        particles.radius[i] = particles.radius[j] * 0.995;
                        break;


                    }
                }
            }
        }}

        ClearBackground(BLACK);

        for(int i{0}; i < max_v_size; i++){
            DrawCircle(particles.x[i], particles.y[i], particles.radius[i], particles.color[i]);
            if(particles.parent_x[i] != 0 && particles.parent_y[i] != 0)
                DrawLineV(Vector2{particles.parent_x[i],particles.parent_y[i]}, Vector2{particles.x[i],particles.y[i]}, particles.color[i]);
        }

        DrawRectangleRec(sliderBounds, LIGHTGRAY);
        DrawRectangleLinesEx(sliderBounds, 2, DARKGRAY);
        float filledWidth = sliderBounds.width * (sliderValue / 100.0f);
        DrawRectangle(sliderBounds.x, sliderBounds.y, filledWidth, sliderBounds.height, BLUE);
        DrawRectangle((int)(sliderBounds.x + filledWidth - 4), (int)(sliderBounds.y - 4), 8, (int)(sliderBounds.height + 8), DARKBLUE);
        DrawText(TextFormat("Noise: %.1f", sliderValue), sliderBounds.x+10, sliderBounds.y+10, 20, DARKGRAY);

        if(IsKeyPressed(KEY_R))Reset();

        EndDrawing();
    }
    CloseWindow();
    return 0;
}
