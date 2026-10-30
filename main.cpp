#include <raylib.h>
#include <vector>

constexpr int window_width{1600};
constexpr int window_height{1000};
constexpr int max_v_size{2000};
constexpr float threshold{15};
constexpr float threshold_sqrd{threshold*threshold};
float noise{15};

class Particle;
std::vector<Particle> particles;


class Particle{
    public:
    Vector2 pos;
    Vector2 velocity;
    float radius{5};
    Color color;
    bool isfrozen{false};
    int hue{0};
    Vector2 parent_pos;

    Particle(){
        pos = {static_cast<float>(GetRandomValue(1, window_width)),static_cast<float>(GetRandomValue(1, window_height))};
        velocity = {static_cast<float>(GetRandomValue(6.0, 14.0)),static_cast<float>(GetRandomValue(2.0, 8.0))};
        color = ColorFromHSV(hue, 0.05, 0.75);
    }
    void Draw()const{
        DrawCircle(pos.x, pos.y, radius, color);

    }

    void Update(){
        if(!isfrozen){
            pos.x += GetRandomValue(-noise, noise);
            pos.y += GetRandomValue(-noise, noise) + 1;
            pos.x = static_cast<float>(static_cast<int>(pos.x + window_width) % window_width);
            if(pos.y > window_height){
                pos.y = 1;
                pos.x = GetRandomValue(1, window_width);
            }
            checkfreeze(particles);
        }
        

    }
    void checkfreeze(std::vector<Particle>& particles){
        float dx,dy,distance;
        for(int i{0}; i<max_v_size;++i){

            if(particles[i].isfrozen){
            dx = pos.x - particles[i].pos.x;
            dy = pos.y - particles[i].pos.y;
            distance = (dx*dx + dy*dy);
            
            if(distance < threshold_sqrd){

                Particle parent = particles[i];
                hue = (parent.hue+4)%360;
                color = ColorFromHSV(static_cast<float>(hue), 1.0f, 1.0f);
                isfrozen=true;

                parent_pos = {parent.pos.x,parent.pos.y};
                radius = parent.radius * 0.995;
                if(radius<0)radius=0.1;

                return;}}
        }
    }

};



void Reset(){
    particles.clear();
      for(int i{0}; i < max_v_size; ++i){
        Particle p;
        particles.push_back(p);
        particles[i].parent_pos = {0,0};
    }

    particles[0].isfrozen=true;
    particles[0].pos = {window_width/2,window_height-2};
    particles[0].color = ColorFromHSV(static_cast<float>(particles[0].hue), 1.0f, 1.0f);
    particles[0].radius = 6;


}

int main(){
    InitWindow(window_width, window_height, "dla");
    SetTargetFPS(60);

    Reset();

    Rectangle sliderBounds{5,window_height-105,250,100};
    bool isdragging = false;
    float& sliderValue = noise;

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


        for(auto& particle : particles){
            particle.Update();
        }

        ClearBackground(BLACK);

        for(const auto& particle : particles){
            particle.Draw();
            if(particle.parent_pos.x != 0 && particle.parent_pos.y != 0){DrawLineV(particle.parent_pos, particle.pos, particle.color);}
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
