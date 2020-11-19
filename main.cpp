#include <SFML/Graphics.hpp>
#include <SFML\Audio.hpp>
#include <Windows.h>
#include "Player.h"
#include "Bullet.h"
#include "background.h"
#include <vector>
#include <sstream>
#include "Menu.h"

using namespace sf;
using namespace std;

static const float VIEW_HEIGHT = 864.0f;
static const float VIEW_WIDHT = 564.0f;

float detalTime = 0.0f;       // ตั้งค่าความเร็วให้ตัวละครเดิน 
float totaltime = 0;		  // ตั้งค่าความเร็วให้ตัวละครเดิน 	
float speed_player = 150.0f;  // ตั้งค่าความเร็วให้ตัวละครเดิน
int   animationFrame = 0; // แฟรมของตัวละคร ที่ 0 

int hit = 0, hit_again = 0;  // rand HP

///////////////////////////////// VIEW //////////////////////////////////////////

//ฟังชั่น ในการเซ้ทค่า display ให้คงที่ตลอดหลังจากปรับขยายหรือหดหน้า
void ResizeView(const sf::RenderWindow& window, sf::View& view)
{
	// aspectRatio คือการเซ้ทค่าของหน้าจอให้คงที่ตลอดหลังจากมีการขยายหน้าจอ เป็นต้น
	float aspectRatio = float(window.getSize().x) / float(window.getSize().y);
	view.setSize(VIEW_HEIGHT * aspectRatio, VIEW_WIDHT * aspectRatio);  // การให้มันคงที่ค่าในแกน x ภาพหน้าจะเหมือนเดิม
}

///////////////////////////////// SHOT //////////////////////////////////////////

class Bullett
{
public:
		
	sf::RectangleShape shot;
	float maxSpeed;
	sf::Vector2f curveVelocity;
	sf::Vector2f position;

	Bullett(float radius = 30.0f)   // เหมือนคำสั่งใน .cpp 
		: curveVelocity(0.0f, 0.0f), maxSpeed(10.0f)

	{			
		this->shot.setSize(sf::Vector2f(radius,radius));
				
	}

};

///////////////////////////////// MAIN //////////////////////////////////////////

int main()
{	
	int  Scorecurrent = 0;

	sf::Font word;
	if (!word.loadFromFile("font/Jelly Crazies.ttf")); {

	}
	
	sf::Text score;
	score.setFont(word);
	score.setCharacterSize(10);
	score.setStyle(sf::Text::Regular);
	score.setPosition(400, 50);
	score.setString("SCORE . ");

	sf::Text scoreCurrent;
	scoreCurrent.setFont(word);
	scoreCurrent.setCharacterSize(10);
	scoreCurrent.setStyle(sf::Text::Regular);
	scoreCurrent.setPosition(500, 50);
	

	///////////////////////////////////////////////////////////////////////////


	int screen = 0; // หน้าจอตอนให้ตัวละครเดินขึ้นมา
	sf::Clock clock;
	srand(time(NULL));
	///////////////////////////////// DISPLAY ///////////////////////////////////////
	
	sf::RenderWindow window(sf::VideoMode(564.0f, 864.0f), "Game_pp" );  //ขนาดหน้าจอ "ชื่อหัวเกม"

	///////////////////////////////// MENU ///////////////////////////////////////

	Menu menu(window.getSize().x,window.getSize().y);

	///////////////////////////////// VIEW //////////////////////////////////////////
	sf::View viewCharacter;
	viewCharacter.setSize(sf::Vector2f(564.0f, 864.0f));
	viewCharacter.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));   // ให้พอดีขนาดหน้าจอ
	//viewCharacter.zoom(1.2f);  // ขยายภาพพื้นหลัง
	//sf::View view(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(564.0f, 864.0f));   // ทำให้การปรับหน้าจอภาพให้มีขนาดตงที่

	/////////////////////////////// BACKGROUND /////////////////////////////////////
	// ลองใส่พื้นหลัง
	//sf::RectangleShape background(sf::Vector2f(564.0f, 864.0f)); // ให้รูปใหญ่เท่าไหร่ ก็เลือกขนาดเอา
	sf::Texture backgroundTexture[2];
	backgroundTexture[0].loadFromFile("./sprite/background_gamed.png");
	//backgroundTexture[1].loadFromFile("./sprite/background_game2.jpg");
	std::vector<Background>backgrounds;
	backgrounds.push_back(Background(&backgroundTexture[0], 110.0f));
	//backgrounds.push_back(Background(&backgroundTexture[1], 80.0f));


	///////////////////////////////// ITEM //////////////////////////////////////////
	// Item
	sf::RectangleShape Heart(sf::Vector2f(42.0f, 43.0f));
	sf::Texture Heartlove;
	Heartlove.loadFromFile("./sprite/Heart.png");
	Heart.setTexture(&Heartlove);
	Heart.setPosition({ 270,340 });
	//Heart.setOrigin(Heart.getSize() / 2.0f);

	///////////////////////////////// HP //////////////////////////////////////////
	//Hp

	sf::RectangleShape HP(sf::Vector2f(230.0f, 70.0f));
	sf::Texture HPplayer;
	HPplayer.loadFromFile("sprite/HP/HP_1_100p.png");
	HP.setTexture(&HPplayer);
	HP.setPosition({ 50,30 });
	
	///////////////////////////////// SHAPE //////////////////////////////////////////
	// เป็นของวงกลม
	sf::CircleShape shape(70,5);  // (ขนาด,มุมที่ให้เกิด เช่นอันนี้เป็น 5 มุม)
	shape.setPosition({ 450,400 });  // ตำแหน่งวงกลม (x,y)
	shape.setFillColor(sf::Color::White);  //สีของรูป

	
	///////////////////////////////// PLAYER //////////////////////////////////////////

	/*sf::RectangleShape player(sf::Vector2f(90.0f, 90.0f)); //ขนาดของตัวละคร
	player.setPosition(240.0f, 1000.0f); //ตน. ของตัวละครตอนเริ่ม
	sf::Texture playerTexture;													 /////////  main ////////
	playerTexture.loadFromFile("sprite/playermainful.png"); // ดึงรูปมาจากในไฟล์
	player.setTexture(&playerTexture); // แอดรูปเข้าไปใน 4 เหลี่ยม
	//256   / 4
	int playerSizeX = playerTexture.getSize().x / 4; // ตัดส่วนของตัวละคร แกน x
	int playerSizeY = playerTexture.getSize().y / 6; // ตัดส่วนของตัวละคร แกน y
	player.setTextureRect(sf::IntRect(0, 0, playerSizeX, playerSizeY));*/
	
	
	sf::Texture playerTexture;													
	playerTexture.loadFromFile("sprite/playermainful.png");
	// animation ใน class		//การแบ่งสโคปของรูปภาพ (4,4)	//ความเร็วในการรันเฟรม (0.5f)	// ความเร็วของตัวละคร (100.0f)
	Player player(&playerTexture, sf::Vector2u(4, 6), 0.2f, 150.0f);



	///////////////////////////////// SHOT //////////////////////////////////////////

	Bullett shot_shot;
	std::vector<Bullett> bullets;
	bullets.push_back(Bullett(shot_shot));

	sf::Vector2f playerCenter;
	sf::Vector2f mousePosition;
	sf::Vector2f aimDirection;
	sf::Vector2f aimDirectionNormalize;
	window.setFramerateLimit(60);

	// delay กระสุน
	sf::Clock shotclock;
	float shotDelay = shotclock.getElapsedTime().asSeconds();

	// ใส่รูป weapon 
	sf::Texture shottexture;
	shottexture.loadFromFile("sprite/weapon.png");
	// shot_shot ก่อนแล้ว เข้าไปในกรอบชื่อ shot แล้วใส่รูปเข้าไป 
	shot_shot.shot.setTexture(&shottexture);


	///////////////////////////////// SOUND //////////////////////////////////////////
	
	sf::Music start_sound; // เพลงเริ่มเกม
	if (!start_sound.openFromFile("sound/songstart.ogg"))
	{
		printf("ERROR\n");
	}
	start_sound.play(); //เปิดเพลง

	///////////////////////////////// SHOT SOUND //////////////////////////////////////////

	sf::SoundBuffer soundeffect;
	soundeffect.loadFromFile("sound/Soundshot.wav");
	sf::Sound shooteffect;
	shooteffect.setBuffer(soundeffect);

	///////////////////////////////// pickup SOUND //////////////////////////////////////////

	sf::SoundBuffer soundpick;
	soundpick.loadFromFile("sound/pickup_item.wav");
	sf::Sound Pickupsound;
	Pickupsound.setBuffer(soundpick);


	//******************************************************************************

	while (window.isOpen())
	{
		shotDelay = shotclock.getElapsedTime().asSeconds();

		/////////////////////////////  เลื่อน PLAYER  //////////////////////////////////////////
		/*
		if (screen == 0) {
			if (player.getPosition().y >= 720) {   // ถ้าตำแหน่งน้อยกว่า 721 ให้ตัวละครเดินขึ้นมาเองจนไม่น้อยกว่า 721
				player.move(0.0f, - 5.0f);
			}
			else {
				screen = 1;
			}
		}

		if (TRUE) {  // ให้หันหน้าอยู่เสมอ
			window.draw(player);
			player.setTextureRect(sf::IntRect(playerSizeX * animationFrame, playerSizeY * 4, playerSizeX, playerSizeY));
		}

		if (screen == 1) {   // เดี๋ยวเอาการรับค่าปุ่มการเดินมาใส่ เพราะตอนเริ่มเกมจะได้กดปุ่มไม่ได้


			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				if (player.getPosition().x < 380) {

					player.move(speed_player * detalTime, 0.f * speed_player);
					player.setTextureRect(sf::IntRect(playerSizeX * animationFrame, playerSizeY * 3, playerSizeX, playerSizeY));  //ปรับให้ตัวละครเดินตามทิศทาง
					
				}
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				if (player.getPosition().x > 100) {

					player.move(-speed_player * detalTime, 0.f * speed_player);
					player.setTextureRect(sf::IntRect(playerSizeX * animationFrame, playerSizeY * 2, playerSizeX, playerSizeY));
					
				}
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				if (player.getPosition().y > 120) {

					player.move(0.f * speed_player, -speed_player * detalTime);
					player.setTextureRect(sf::IntRect(playerSizeX * animationFrame, playerSizeY * 4, playerSizeX, playerSizeY));
					
				}
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				if (player.getPosition().y < 730) {

					player.move(0.f * speed_player, speed_player * detalTime);
					player.setTextureRect(sf::IntRect(playerSizeX * animationFrame, playerSizeY * 1, playerSizeX, playerSizeY));
					
				}

			}

		}
		*/


			detalTime = clock.restart().asSeconds(); // คำสั่งหน่วงให้น้องเดินช้า
			
			sf::Event evnt;
			while (window.pollEvent(evnt))	// while นี้ทำให้สามารถใช้เม้ากดเต็มจอหรือใช้ขยาย กว้างยาวได้
			{
				switch (evnt.type)
				{

				case sf::Event::KeyReleased:
					switch (evnt.key.code)
					{
					
					case sf::Keyboard::Up:
						menu.MoveUp();
						break;

					case sf::Keyboard::Down:
						menu.MoveDown();
						break;

					case sf::Keyboard::Return:
						switch (menu.GetPressedItem())
						{
						case 0:    // การรับค่าหน้า menu เข้ามา 
					
							printf("Play button has been pressed\n");			
							break;

						case 1:
							printf("Options button has been pressed\n");
							break;

						case 2:  // [2] array ที่ 2 จะให้ออกจากการทำงานทั้งหมด
							window.close();
							break;
						}
					}

					break;

				case sf::Event::Closed:

					window.close();
					break;

				case sf::Event::Resized:

					ResizeView(window, viewCharacter);
					break;

				}
			}

			////////////////////////////// RAND HP //////////////////////////////////////////	

			int rand_x = (rand() % 200) + 180;  // rand หัวใจ 
			int rand_y = (rand() % 500) + 100;
			
			/*if (Heart.getGlobalBounds().intersects(player.getGlobalBounds()))
			{
				
				Heart.setPosition(Vector2f(rand_x, rand_y));
				//Heart.setFillColor(sf::Color::Transparent);
				HPplayer.loadFromFile("sprite/HP/HP_2_80p.png");   // ให้เลือด ลด
				HP.setTexture(&HPplayer);
				Pickupsound.play();
				HP.setPosition({ 50,30 });
				//hit_again=hit+1;
				Scorecurrent+=5;*/

				/*if (hit_again==2)
				{
					HPplayer.loadFromFile("sprite/HP/HP_3_70p.png");   // ให้เลือด ลด
					HP.setTexture(&HPplayer);
					HP.setPosition({ 50,30 });
				}*/

			//}

			///////////////////////////////// LOOP BG //////////////////////////////////////////	

			for (Background& background : backgrounds) {
				background.Update(detalTime);
			}

			for (Background& background : backgrounds) {   // ลูปให้รูปวน 
				background.Draw(window);
			}

			/// ///////////////////////////////////////////////////////////////////////////////

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				window.close();  // กด esc เพื่อออกจากโปรแกรม
			}


			/*totaltime += detalTime; // ให้น้องวิ่งเป็นเอนิเมชั่น
			if (totaltime >= 0.2) {  // ทำให้น้องสิ่งเร็วหรือช้า แบบเอนิเมชั่น
				totaltime -= 0.2;
				animationFrame++;   // ให้น้องตัวละครไม่เดินเร็วเกิน เป็นการเซ้ทค่าความหน่วงให้น้อง
			}

			if (animationFrame > 3) {   // ทำเอนิเมชั่นให้มันเคลื่อนไหว ตลอด
				animationFrame = 0;
			}*/


			//////////////////////// 	Update shot  /////////////////////////////////
		
			playerCenter = sf::Vector2f(player.getPosition().x-15, player.getPosition().y+10); // ตำแหน่งกระสุนออกจากตัว

			mousePosition = sf::Vector2f(Mouse::getPosition(window));
			
			aimDirection = mousePosition - playerCenter;  // ทิศทางการยิง
			aimDirectionNormalize = aimDirection / sqrt(pow(aimDirection.x, 2) + pow(aimDirection.y, 2));
			
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)&&shotDelay>0.25)    // Mouse ยิง
			{
				shot_shot.shot.setPosition(playerCenter);
				shot_shot.curveVelocity = aimDirectionNormalize * shot_shot.maxSpeed;
				bullets.push_back(Bullett(shot_shot));
				shooteffect.play();
				shotDelay = shotclock.restart().asSeconds();

			}

			
			for (size_t i = 0; i < bullets.size(); i++)
			{
			
				bullets[i].shot.move(bullets[i].curveVelocity);
			}

			for (size_t i = 0; i < bullets.size(); i++)   // ลูปการวาดกระสุน 
			{
				if (i != 0) 
				{	
					window.draw(bullets[i].shot);
				}
				
				
			}

			stringstream scoreShow;
			scoreShow << Scorecurrent;
			scoreCurrent.setString(scoreShow.str().c_str());

			/////////////////////////////  DRAW //////////////////////////////////////////
			player.Draw(window);
			player.Update(detalTime);
			window.draw(score);
			window.draw(scoreCurrent);
			menu.draw(window);
			window.draw(Heart);
			window.draw(HP);	 
			//window.draw(player);   main
			window.display();
			window.clear(sf::Color::Transparent);
			
		}
	
	return 0;

}



