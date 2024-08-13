
#include "Handler.h"

class A {
public:
    A() {

    }
    void test() {
        print();
    }
    virtual void print() {
        std::cout << "A\n";
    }
};

class B : public A {
public:
    B() {

    }

    void print() {
        std::cout << "B\n";
    }
};

int main()
{
    sf::RenderTexture renderTexture;
    if (!renderTexture.create(SCREEN_WIDTH, SCREEN_HEIGHT)) {
        return EXIT_FAILURE;
    }

    sf::Texture texture;
    if (!texture.loadFromFile(FILENAME)) {
        return -1;
    }

    sf::Sprite robot(texture);

    float w = robot.getLocalBounds().width;
    float h = robot.getLocalBounds().height;

    float w_req = SCREEN_WIDTH - 2 * BOX_WIDTH;
    float scaleBy = (w_req / w);
    robot.setOrigin(sf::Vector2f(w, h));
    robot.rotate(180);
    robot.setPosition(sf::Vector2f(BOX_WIDTH, BOX_HEIGHT));
    robot.setScale(sf::Vector2f(scaleBy, scaleBy));

    Handler handler = Handler(BALL_COUNT);



   

    int t = FRAME_COUNT;
    while(t--) {
        
        
        renderTexture.clear(sf::Color::Black);

      
        int buff = SUBFRAMES;



        if (t % SINGLE_SPAWN_FRAME_INTERVAL == 0 ) {
            handler.spawnSingle(sf::Vector2f(SCREEN_WIDTH / 2-200, SCREEN_HEIGHT - 100.0f), sf::Vector2f(40,0)  ,BALL_RADIUS);
            handler.spawnSingle(sf::Vector2f(SCREEN_WIDTH / 2-100, SCREEN_HEIGHT - 100.0f), sf::Vector2f(-40, 0),BALL_RADIUS);
            handler.spawnSingle(sf::Vector2f(SCREEN_WIDTH / 2    , SCREEN_HEIGHT - 100.0f), sf::Vector2f(-40, 0), BALL_RADIUS);
            handler.spawnSingle(sf::Vector2f(SCREEN_WIDTH / 2+100, SCREEN_HEIGHT - 100.0f), sf::Vector2f(40, 0) ,BALL_RADIUS);
            handler.spawnSingle(sf::Vector2f(SCREEN_WIDTH / 2+200, SCREEN_HEIGHT - 100.0f), sf::Vector2f(-40, 0),BALL_RADIUS);
        }
        while(buff--) handler.update((FRAME_TIME)/SUBFRAMES);
        
        renderTexture.draw(robot);
        handler.draw(renderTexture);

        
        //sf::Image screenshot = renderTexture.getTexture().copyToImage();
        //screenshot.flipVertically();
        //std::string filename = "video\\frame" + std::to_string(FRAME_COUNT-t) + ".png"; // Change the filename format as needed
        //screenshot.saveToFile(filename);
        std::cout << "Pseudo Frame: "<<FRAME_COUNT-t<<"\r";
    }
    std::cout << "\n";
    
    handler.registerColor(renderTexture);
    handler.resetHandler();
    handler.printColorMap();

    t = FRAME_COUNT;
    while (t--) {


        renderTexture.clear(sf::Color::Black);


        int buff = SUBFRAMES;



        if (t % SINGLE_SPAWN_FRAME_INTERVAL == 0) {
            handler.spawnSingle(sf::Vector2f(SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT - 100.0f), sf::Vector2f(40, 0), BALL_RADIUS );
            handler.spawnSingle(sf::Vector2f(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT - 100.0f), sf::Vector2f(-40, 0), BALL_RADIUS);
            handler.spawnSingle(sf::Vector2f(SCREEN_WIDTH / 2      , SCREEN_HEIGHT - 100.0f), sf::Vector2f(-40, 0), BALL_RADIUS);
            handler.spawnSingle(sf::Vector2f(SCREEN_WIDTH / 2 + 100, SCREEN_HEIGHT - 100.0f), sf::Vector2f(40, 0), BALL_RADIUS );
            handler.spawnSingle(sf::Vector2f(SCREEN_WIDTH / 2 + 200, SCREEN_HEIGHT - 100.0f), sf::Vector2f(-40, 0), BALL_RADIUS);
        }
        while (buff--) handler.update((FRAME_TIME) / SUBFRAMES);


        handler.draw(renderTexture);


        sf::Image screenshot = renderTexture.getTexture().copyToImage();
        screenshot.flipVertically();
        std::string filename = "video\\frame" + std::to_string(FRAME_COUNT-t) + ".png"; // Change the filename format as needed
        screenshot.saveToFile(filename);
        std::cout << "Frame: " << FRAME_COUNT - t << "\r";
    }
    return 0;
}