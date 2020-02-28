#ifndef UNICODE
#define UNICODE
#endif

#include <Windows.h>
#include "d2d1.h"
#pragma comment(lib, "d2d1.lib")
#include<dwrite.h>
#pragma comment(lib, "dwrite.lib")
#include<vector>
#include<iostream>
#include<thread>
#include<mutex>
#include<string>
#include<chrono>
#include<cmath>
#include<Windowsx.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
RECT clientWindow;
RECT fullWindow;


ID2D1Factory* pFactory;
IDWriteFactory* pWriteFactory;
IDWriteTextFormat* pWrite;
ID2D1SolidColorBrush* pBrush;
ID2D1HwndRenderTarget* pRenderTarget;

struct COLOR3 {
	D2D1_COLOR_F red = D2D1::ColorF(1.0f, 0, 0);
	D2D1_COLOR_F green = D2D1::ColorF(0, 1.0f, 0);
	D2D1_COLOR_F blue = D2D1::ColorF(0, 0, 1.0f);
	D2D1_COLOR_F orange = D2D1::ColorF(1.0f, 0.5f, 0);
	D2D1_COLOR_F yellow = D2D1::ColorF(1.0f, 1.0f, 0);
	D2D1_COLOR_F purple = D2D1::ColorF(0.75f, 0, 1.0f);
	D2D1_COLOR_F pink = D2D1::ColorF(1.0f, 0.5f, 0.5f);
	D2D1_COLOR_F skyblue = D2D1::ColorF(0, 1.0f, 1.0f);
	D2D1_COLOR_F black = D2D1::ColorF(0, 0, 0);
	D2D1_COLOR_F brown = D2D1::ColorF(0.50f, 0, 0);
	D2D1_COLOR_F white = D2D1::ColorF(1.0f, 1.0f, 1.0f);
	D2D1_COLOR_F grey = D2D1::ColorF(0.5f, 0.5f, 0.5f);
	D2D1_COLOR_F darkGrey = D2D1::ColorF(0.25f, 0.25f, 0.25f);
};
COLOR3 color3;



bool wait(double seconds) {
	int milliseconds = seconds * 1000;
	std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
	return true;
}
bool waitFor(bool* condition) {
	while (!*condition) {
		//stay in loop
	}
	return true;
}
double keepSign(double signSrc, double signDest) {
	if (signSrc >= 0) {
		signDest = std::abs(signDest);
	}
	else {
		if (signDest > 0) {
			signDest *= -1;
		}
	}
	return signDest;
}
void print(float output) {
	std::wstring buffer = std::to_wstring(output);
	wchar_t newStr[100];
	for (int i = 0; i <= buffer.size(); ++i) {
		newStr[i] = buffer[i];

	}
	OutputDebugStringW(newStr);
	OutputDebugStringW(L"\n");
}
void print(std::wstring output) {
	std::wstring buffer = output;
	wchar_t newStr[100];
	for (int i = 0; i <= buffer.size(); ++i) {
		newStr[i] = buffer[i];

	}
	OutputDebugStringW(newStr);
	OutputDebugStringW(L"\n");
}
void redraw(HWND* hwnd) {
	if (hwnd != nullptr) {
		InvalidateRect(*hwnd, NULL, TRUE);
		UpdateWindow(*hwnd);
	}
}
bool signChanged(double before, double after) {
	if ((before > 0 && after > 0) || (before < 0 && after < 0)) {
		return false;
	}
	else {
		return true;
	}
}
void softEqual(float* acceleration, float* velocity, float* maxVelocity, float* start, float* goal, bool* killThread = nullptr) {
	double difference_before = *start - *goal;
	double difference_now = *start - *goal;
	*velocity = 0;
	while (wait(.033) && (killThread == nullptr || !*killThread)) {
		*velocity += *acceleration;
		if (*velocity > * maxVelocity) {
			*velocity = *maxVelocity;
		}
		if (difference_now < 0) {
			if (*start + *velocity > * goal) {
				//*start = *goal;
				*start += *velocity;
				*velocity = 0;

			}
			else {
				*start += *velocity;

			}
		}
		else if (difference_now > 0) {
			if (*start - *velocity < *goal) {
				//*start = *goal;
				*start -= *velocity;
				*velocity = 0;
			}
			else {
				*start -= *velocity;
			}
		}
		difference_now = *start - *goal;
	}
	return;
}

class Canvas {
public:
	float x = 0;
	float y = 0;
	float width = 25.00f;
	float height = 25.00f;
	FLOAT borderWidth = 0;
	D2D1_COLOR_F color = D2D1::ColorF(1.0f, 0, 0);
	D2D1_COLOR_F borderColor = D2D1::ColorF(0, 0, 0);
	D2D1_COLOR_F textColor = color3.black;
	HWND* canvasHwnd=nullptr;
	bool legal = false;

	std::vector<Canvas*>objects;


	void draw(Canvas canvas) {};
	void initialize(HWND* hwnd) {
		canvasHwnd = hwnd;
		RECT rect;
		GetClientRect(*hwnd, &rect);
		HRESULT hr = S_OK;
		hr = pFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(*hwnd, D2D1::SizeU(rect.right, rect.bottom)), &pRenderTarget);
		if (SUCCEEDED(hr)) {
			hr = pRenderTarget->CreateSolidColorBrush(color, &pBrush);
			if (SUCCEEDED(hr)) {
				legal = true;
				pRenderTarget->BeginDraw();
				
				
			}
		}
	}
	void quick(int newX, int newY, int newWidth, int newHeight, D2D1_COLOR_F newColor) {
		x = newX;
		y = newY;
		width = newWidth;
		height = newHeight;
		color = newColor;
	}
	void destroy() {
		pRenderTarget->EndDraw();
		if (pRenderTarget != NULL) {
			pRenderTarget->Release();
		}
		if (pBrush != NULL) {
			pBrush->Release();
		}
	}
	void SetBackground(D2D1_COLOR_F backgroundColor) {
		if (legal) {
			D2D1_RECT_F rectF{ clientWindow.left, clientWindow.top, clientWindow.right, clientWindow.bottom };
			pBrush->SetColor(backgroundColor);

			pRenderTarget->DrawRectangle(rectF, pBrush, 0, NULL);
			pRenderTarget->FillRectangle(rectF, pBrush);


		}
	}

};//end of Canvas class
Canvas canvas;

class Entity;
std::vector<Entity*>Objects;
struct SELECTED {
	Entity* object = nullptr;
	bool isDragging = false;
	float originalBorderWidth = 0;
	D2D1_COLOR_F originalBorderColor = color3.black;
};
SELECTED selected;
bool isEditing = true;

class Entity : public Canvas {
public:
	std::wstring name = L"Default";
	std::wstring classID = L"None";
	float vy = 0;
	float vx = 0;
	float left = 0;
	float right = 0;
	float bottom = 0;
	float top = 0;
	float walkSpeed = 5;
	float fontSize = 12.0f;
	float centerX = 0;
	float centerY = 0;
	float momemtum = 0;
	float presence = 1.0f;
	float radius = 0;
	int health = 100;
	int jumpPower = 120;
	bool anchored = true;
	bool canCollide = true;
	bool hidden = false;
	bool isFalling = false;
	bool isWalking = false;
	bool matchParentWidth = false;
	bool matchParentHeight = false;
	bool isGUI = false;
	bool lostFocus = false;
	bool cameraThreadTerminated = true;
	bool isTextBox = false;
	bool isInputBox = false;
	enum class SHAPE { Rectangle, Circle };
	enum class FOCUS { Center, Left, Top, Right, Bottom };
	SHAPE shape = SHAPE::Rectangle;
	FOCUS focus = FOCUS::Center; //anchor point if entity has a parent
	DWRITE_TEXT_ALIGNMENT textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
	DWRITE_PARAGRAPH_ALIGNMENT paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
	DWRITE_FONT_WEIGHT fontWeight = DWRITE_FONT_WEIGHT_REGULAR;
	DWRITE_FONT_STYLE fontStyle = DWRITE_FONT_STYLE_NORMAL;
	DWRITE_FONT_STRETCH fontStretch = DWRITE_FONT_STRETCH_NORMAL;
	WCHAR text[100] = L"TextBox";
	Entity* parent = nullptr;
	POINT direction{ 0,0 };
	Canvas* canvas = nullptr;
	
	
	std::vector<Entity*>children;
	wchar_t fontFamily[50] = L"Segoe";

	Entity(Canvas* MainCanvas,float newX, float newY, float newWidth, float newHeight, SHAPE newShape, D2D1_COLOR_F newColor, float newBorderSize = 0) {
		x = newX;
		y = newY;
		width = newWidth;
		height = newHeight;
		shape = newShape;
		color = newColor;
		Objects.push_back(this);
		canvas = MainCanvas;
		updateDimensions();

	}
	void draw() {
		if (parent != nullptr) {
			if (focus == FOCUS::Center) {
				centerX = (parent->left + parent->right) / 2;
				centerY = (parent->bottom + parent->top) / 2;
			}
			else if (focus == FOCUS::Top) {
				centerX = (parent->left + parent->right) / 2;
				centerY = parent->top;
			}
			else if (focus == FOCUS::Left) {
				centerX = parent->left;
				centerY = (parent->bottom + parent->top) / 2;
			}
			else if (focus == FOCUS::Right) {
				centerX = parent->right;
				centerY = (parent->bottom + parent->top) / 2;
			}
			else if (focus == FOCUS::Bottom) {
				centerX = (parent->left + parent->right) / 2;
				centerY = parent->bottom;
			}
		}
		if (!hidden) {

			if (canvas->legal) {
				pBrush->SetColor(borderColor);
				D2D1_MATRIX_3X2_F matrix = D2D1::Matrix3x2F::Rotation(radius, D2D1::Point2F((left + right) / 2, (top + bottom) / 2));
				pRenderTarget->SetTransform(matrix);
				switch (shape) {
				case SHAPE::Rectangle: {
					D2D1_RECT_F rectF{ x - width + centerX,y + height + centerY,x + width + centerX,y - height + centerY };
					pRenderTarget->DrawRectangle(rectF, pBrush, borderWidth, NULL);
					color.a = presence;
					pBrush->SetColor(color);
					pRenderTarget->FillRectangle(rectF, pBrush);
				}

				}

				if (isTextBox) {
					UINT32 stringLength = wcslen(text);

					const D2D1_RECT_F writeRect = { left,top,right,bottom };
					HRESULT hr = pWriteFactory->CreateTextFormat(
						fontFamily,
						NULL,
						fontWeight,
						fontStyle,
						fontStretch,
						fontSize,
						L"en-us",
						&pWrite
					);
					pWrite->SetTextAlignment(textAlignment);
					pWrite->SetParagraphAlignment(paragraphAlignment);
					pBrush->SetColor(textColor);
					if (SUCCEEDED(hr)) {
						pRenderTarget->DrawTextW(
							text,
							stringLength,
							pWrite,
							writeRect,
							pBrush
						);
						pWrite->Release();

					}
				}

				

			}
		}
		pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		updateDimensions();
	}
	void addText(std::wstring newText) {
		isTextBox = true;
		for (int i = 0; i < wcslen(text); ++i) {
			print(i);
			if (i < newText.size()) {
				text[i] = newText[i];
			}
			else {
				text[i] = NULL;
			}
		}
		
		
	}
	void setXY(float setX, float setY) {
		x = setX;
		y = setY;
		redraw(canvas->canvasHwnd);
	}
	void move(float movex, float movey) {
		if (movex != 0 || movey != 0) {
			x += movex;
			y += movey;
			updateDimensions();
			redraw(canvas->canvasHwnd);
		}

	}
	Entity* testMovex(float movex) {
		Entity* result = nullptr;
		for (int i = 0; i <= Objects.size() - 1; ++i) {
			if (Objects[i] != this) {
				if (((
					(
						Objects[i]->top <= bottom //object top is above this bottom
						&&
						Objects[i]->top >= top //object top is below this top
						)
					||
					(
						Objects[i]->bottom <= bottom  //object bottom is above this bottom
						&&
						Objects[i]->bottom >= top  //object bottom is below this top
						)
					)
					&& ((
						Objects[i]->left >= left + movex
						&&
						Objects[i]->left <= right + movex
						)
						||
						(
							Objects[i]->right >= left + movex
							&&
							Objects[i]->right <= right + movex
							)
						))

					|| ((
					(
						this->top >= Objects[i]->top
						&&
						this->top <= Objects[i]->bottom
						)
						||
						(
							this->bottom <= Objects[i]->bottom
							&&
							this->bottom >= Objects[i]->top
							)
						)
						&& ((
							this->left + movex <= Objects[i]->right
							&&
							this->left + movex >= Objects[i]->left
							)
							||
							(
								this->right + movex >= Objects[i]->left
								&&
								this->right + movex <= Objects[i]->right
								)
							))) {
					result = Objects[i];

					break;
				}

			}
		}
		return result;
	}
	Entity* testMovey(float movey) {
		Entity* result = nullptr;
		for (int i = 0; i <= Objects.size() - 1; ++i) {
			if (Objects[i] != this) {
				if (((
					(
						Objects[i]->top <= bottom + movey //object top is above this bottom
						&&
						Objects[i]->top >= top + movey //object top is below this top
						)
					||
					(
						Objects[i]->bottom <= bottom + movey //object bottom is above this bottom
						&&
						Objects[i]->bottom >= top + movey //object bottom is below this top
						)
					)
					&& ((
						Objects[i]->left >= left
						&&
						Objects[i]->left <= right
						)
						||
						(
							Objects[i]->right >= left
							&&
							Objects[i]->right <= right
							)
						))

					|| ((
					(
						this->top + movey >= Objects[i]->top
						&&
						this->top + movey <= Objects[i]->bottom
						)
						||
						(
							this->bottom + movey <= Objects[i]->bottom
							&&
							this->bottom + movey >= Objects[i]->top
							)
						)
						&& ((
							this->left <= Objects[i]->right
							&&
							this->left >= Objects[i]->left
							)
							||
							(
								this->right >= Objects[i]->left
								&&
								this->right <= Objects[i]->right
								)
							))) {
					result = Objects[i];

					break;
				}

			}
		}
		return result;
	}
	Entity* getHitTarget() {
		Entity* hitTarget = testMovex(0);
		if (hitTarget != nullptr) {
			return hitTarget;
		}
		else {
			hitTarget = testMovey(0);
			if (hitTarget != nullptr)return hitTarget;
		}

		return nullptr;

	}
	void resize(float resizeX, float resizeY, bool rigid = true, FOCUS focusX = FOCUS::Right, FOCUS focusY = FOCUS::Top) {
		width += resizeX;
		height += resizeY;
		if (rigid) {
			switch (focusX) {
			case FOCUS::Left: {
				x -= (resizeX);
				break;
			}
			case FOCUS::Right: {
				x += (resizeX);
				break;
			}

			}
			switch (focusY) {
			case FOCUS::Top: {
				y -= (resizeY);
				break;
			}
			case FOCUS::Bottom: {
				y += (resizeY);
				break;
			}
			}
		}
		for (int i = 0; i < children.size(); ++i) {
			Entity* child = children[i];
			if (child->matchParentWidth) {
				child->resize(resizeX, 0, rigid, focusX, focusY);
			}
			if (child->matchParentHeight) {
				child->resize(0, resizeY, rigid, focusX, focusY);
			}
		}
	}
	void unstuck() {
		Entity* hitX = testMovex(0);
		Entity* hitY = testMovey(0);
		if (hitY != NULL) {
			float topDiff = std::abs(top - hitY->bottom);
			float bottomDiff = std::abs(hitY->top - bottom);
			if (topDiff < bottomDiff) {
				y += topDiff;
			}
			else if (bottomDiff < topDiff) {
				y -= bottomDiff;
			}
		}
	}
	void processPhysics() {
		if (!anchored) {
			isFalling = true;
			vy += 100;
			float moveIncrement = vy / 30;
			Entity* hitTarget = testMovey(moveIncrement);
			if (hitTarget != NULL) {
				moveIncrement = hitTarget->top - bottom;
				vy = 0;
				isFalling = false;
			}
			move(0, moveIncrement);
			unstuck();
		}

	}
	void join(Entity& newParent) {
		if (parent != &newParent) {
			unjoin();

			parent = &newParent;
			bool childExists = false;
			for (int i = 0; i < newParent.children.size(); ++i) {
				if (newParent.children[i] == this) {
					childExists = true;
					break;
				}
			}
			if (!childExists) {
				newParent.children.push_back(this);
			}
		}
	}
	void unjoin() {
		if (parent != nullptr) {
			std::vector<Entity*>newChildren;
			for (int i = 0; i < parent->children.size(); ++i) {
				if (parent->children[i] != this) {
					newChildren.push_back(parent->children[i]);
				}
			}
			parent->children = newChildren;
		}
		parent = nullptr;
		centerY = 0;
		centerX = 0;
	}
	void remove() {
		std::vector<Entity*>newObjects;
		for (int i = 0; i < Objects.size(); ++i) {
			if (Objects[i] != this) {
				newObjects.push_back(Objects[i]);
			}
		}
		hidden = true;
		Objects = newObjects;
		for (int i = 0; i < children.size(); ++i) {
			children[i]->remove();
		}
	}
	void trackMovement( bool newThread=true) {
		if (newThread) {
		std::thread startThread{ &Entity::trackMovement,this,false };
		startThread.detach();
		return;
		}
		else {
			float static_x = x;
			float static_y = y;
			while (wait(.05)) {
				
				float change_x = x - static_x;
				float change_y = y - static_y;
				if (change_x > 0) {
					direction.x = 1;
				}
				else if (change_x < 0) {
					direction.x = -1;
				}
				else {
					direction.x = 0;
				}

				if (change_y > 0) {
					direction.y = 1;
				}
				else if (change_y < 0) {
					direction.y = -1;
				}
				else {
					direction.y = 0;
				}
				momemtum = abs(change_x) + abs(change_y);

				static_x = x;
				static_y = y;
			}
		}
		
	}
	void liveTrack(RECT* window, bool* killThread=nullptr, bool newThread = true) {
		if (newThread) {
			std::thread startThread{ &Entity::liveTrack,this,window,killThread,false };
			startThread.detach();
			return;
		}
		else {
			POINT cursorPos;
			while (killThread==nullptr || !*killThread) {
				//wait(.5);
				GetCursorPos(&cursorPos);
				float x_static = cursorPos.x - window->left;
				float y_static = cursorPos.y - window->top;
				if (x_static != x || y_static != y) {

					x = x_static;
					y = y_static;
					updateDimensions();
					redraw(canvas->canvasHwnd);
					//print(x);
				}
			}

		}
	}
	void fade(float goal=0,int increment=5,bool newThread=true) {
		if (newThread) {
			std::thread startThread{ &Entity::fade,this,goal,increment,false };
			startThread.detach();
			return;
		}

		while (presence != goal && wait(.1)) {
			if (presence > goal) {
				presence -= (increment * .01);
				if (presence <= goal) { 
					presence = goal; 
					break;
				}
			}
			else if (presence < goal) {
				presence += (increment * .01);
				if (presence >= goal) {
					presence = goal;
					break;
				}
			}
		}
	}

	bool shakeAnimated = false;
	void shake(int force =5, int duration=10, bool shakeX=true, bool shakeY=true, bool newThread=true) {
		if (!shakeAnimated) {
			shakeAnimated = true;
			if (newThread) {
				shakeAnimated = false;
				std::thread newThread{ &Entity::shake,this,force,duration,true,true,false };
				newThread.detach();
				return;
			}
			else {
				float moveAmount = 0.3f;
				int increment = force;
				float animationTime = .01;
				for (int i = 0; i < duration; ++i) {
					for (int i = 0; i < increment; ++i) {
						wait(animationTime);
						if (shakeX) move(moveAmount, 0);
						if (shakeY) move(0, moveAmount);
					}
					for (int i = 0; i < increment; ++i) {
						wait(animationTime);
						if (shakeX) move(moveAmount * -2, 0);
						if (shakeY) move(0, moveAmount * -2);
					}
					for (int i = 0; i < increment; ++i) {
						wait(animationTime);
						if (shakeX) move(moveAmount, 0);
						if (shakeY) move(0, moveAmount);
					}
				}
				shakeAnimated = false;
			}
		}
	}


	//Player mechanics
	void walkRight() {
		if (isWalking) {
			return;
		}
		isWalking = true;
		while (isWalking) {
			move(walkSpeed, 0);
			std::this_thread::sleep_for(std::chrono::milliseconds(33));
		}
	}
	void walkLeft() {
		if (isWalking) {
			return;
		}
		isWalking = true;
		while (isWalking) {
			move(walkSpeed * -1, 0);
			std::this_thread::sleep_for(std::chrono::milliseconds(33));
		}
	}
	void stop() {
		isWalking = false;
	}
	void jump() {
		if (!isFalling) {
			isFalling = true;
			vy = 0;
			vy = std::abs(jumpPower * 10) * -1;
		}
	}

	//Camera mechanics
	void focusOn(float* focusX, float* focusY, HWND* hwnd, bool startNewThread = true) {
		if (startNewThread) {
			std::thread newThread{ &Entity::focusOn,this,focusX,focusY,hwnd,false };
			newThread.detach();
			return;
		}
		lostFocus = true;
		waitFor(&cameraThreadTerminated);

		lostFocus = false;
		cameraThreadTerminated = false;
		RECT rect;
		int maxOffset = 5;
		float acceleration = 0;
		float goalX = x;
		float goalY = y;
		bool startedX = false;
		bool startedY = false;
		float maxAcceleration = 0.15f;
		float maxVelocity = walkSpeed;
		std::thread tx{ softEqual,&acceleration,&vx,&maxVelocity,&x,&goalX,&lostFocus };
		std::thread ty{ softEqual,&acceleration,&vy,&maxVelocity,&y,&goalY,&lostFocus };
		while (!lostFocus) {
			GetClientRect(*hwnd, &rect);
			goalX = rect.right / 2 - *focusX;
			goalY = rect.bottom / 2 - *focusY;

			if (!startedX && abs(goalX - x) >= maxOffset) {
				acceleration = maxAcceleration;
				if (vx >= 0)startedX = true;

			}
			else if (startedX && vx == 0) {
				startedX = false;
				acceleration = 0;
			}

			if (!startedY && abs(goalY - y) >= maxOffset) {
				acceleration = maxAcceleration;
				if (vy >= 0)startedY = true;

			}
			else if (startedY && vy == 0) {
				startedY = false;
				acceleration = 0;
			}
		}
		tx.join();
		ty.join();
		cameraThreadTerminated = true;

	}

	void updateDimensions() {
		// x - width,y + height,x + width,y - height
		left = x - width + centerX;
		top = y - height + centerY;
		right = x + width + centerX;
		bottom = y + height + centerY;

	}
private:



};
void doPhysics() {
	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(33));
		for (int i = 0; i <= Objects.size() - 1; ++i) {
			Objects[i]->processPhysics();
		}
	}
}
//std::thread physics(doPhysics);

class FileWorker {
public:
	HANDLE hFile;
	DWORD bytesRead, bytesToRead, bytesWritten;
	//char buffer[] = "This is a text file";

	void makeFile() {
		hFile = CreateFile(
			TEXT("C:\\Users\\Administrator\\Desktop\\Test.txt"),
			GENERIC_WRITE,
			0,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);

	}
};


//Instance creation
void joinCamera(Entity* camera) {
	while (camera != nullptr && wait(0.1)) {
		for (int i = 0; i < Objects.size(); ++i) {
			if (Objects[i] != camera && Objects[i]->isGUI == false && Objects[i]->parent == nullptr) {
				Objects[i]->join(*camera);
			}
		}
	}
}
enum class Instance {grassBlock,greyBlock,prisoner,guard,cage,camera,adminPanel};
class World {
public:
	Entity* create(Instance instance) {
		switch (instance) {
		case Instance::camera: {
			for (int i = 0; i < Objects.size(); ++i) {
				if (Objects[i]->classID == L"Camera") {
					Objects[i]->unjoin();
					Objects[i]->remove();
				}
			}
			Entity* camera = new Entity(&canvas,0, 0, 1, 1, Entity::SHAPE::Rectangle, color3.white);
			camera->classID = L"Camera";
			std::thread t1{ joinCamera,camera };
			t1.detach();
			return camera;
			break;
		}
		case Instance::grassBlock: {
			Entity* dirt = new Entity(&canvas,0, 0, 100, 35, Entity::SHAPE::Rectangle, color3.brown);
			dirt->classID = L"Dirt";
			Entity* grass = new Entity(&canvas,0, -10, dirt->width, 10, Entity::SHAPE::Rectangle, color3.green);
			grass->join(*dirt);
			grass->matchParentWidth = true;
			grass->classID = L"Grass";
			grass->focus = Entity::FOCUS::Top;
			return dirt;
			break;
		}
		case Instance::greyBlock: {
			Entity* block = new Entity(&canvas,0, 0, 0, 0, Entity::SHAPE::Rectangle, color3.grey,1.0f);
			block->classID = L"greyBlock";
			return block;
			break;
		}
		case Instance::cage: {
			Entity* cage = new Entity(&canvas,0, 0, 40, 25, Entity::SHAPE::Rectangle, color3.darkGrey);
			cage->classID = L"Cage";
			cage->addText(L"Cage");
			return cage;
			break;
		}
		case Instance::adminPanel: {
			Entity* main = new Entity(&canvas, 0, 0, 100, 600, Entity::SHAPE::Rectangle, color3.grey, 0);
			Entity* nameInputText = new Entity(&canvas, 0, 0, main->width, 35, Entity::SHAPE::Rectangle, color3.darkGrey, 0);
			Entity* classInputText = new Entity(&canvas, 0, 0, main->width, 35, Entity::SHAPE::Rectangle, color3.darkGrey, 0);
			Entity* colorInputText = new Entity(&canvas, 0, 0, main->width, 35, Entity::SHAPE::Rectangle, color3.darkGrey, 0);
		}
		}
	}
};
World world;

Entity* camera = world.create(Instance::camera);
Entity* backWall = world.create(Instance::greyBlock);
Entity* leftWall = world.create(Instance::greyBlock);
Entity* rightWall = world.create(Instance::greyBlock);
Entity* upperLeftWall = world.create(Instance::greyBlock);
Entity* upperRightWall = world.create(Instance::greyBlock);
Entity* cage = world.create(Instance::cage);
Entity player(&canvas,712, 100, 5, 5, Entity::SHAPE::Rectangle, color3.red, 1.0f);

HINSTANCE thisInstance;
int windowMaxWidth = 800;
int windowMaxHeight = 600;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PWSTR pCmdLine, int nCmdShow) {
	player.classID = L"Player";
	player.isGUI = true;
	
	player.hidden = true;

	backWall->width = windowMaxWidth;
	backWall->x = windowMaxWidth/2;
	backWall->height = 25;
	backWall->y = windowMaxHeight- backWall->height*2;

	leftWall->width = backWall->height;
	leftWall->height = windowMaxWidth;
	leftWall->x = leftWall->width;
	leftWall->y = windowMaxHeight/2;

	rightWall->width = backWall->height;
	rightWall->height = leftWall->height;
	rightWall->x = rightWall->height - leftWall->width;
	rightWall->y = leftWall->y;

	upperLeftWall->width = 150;
	upperLeftWall->height = backWall->height;
	upperLeftWall->x = 200;
	upperLeftWall->y = upperLeftWall->height;
	
	upperRightWall->width = upperLeftWall->width;
	upperRightWall->height = backWall->height;
	upperRightWall->x = windowMaxWidth - 200;
	upperRightWall->y = upperRightWall->height;

	cage->x = 400;
	cage->y = 10;
	
	thisInstance = hInstance;
	wchar_t CLASSNAME[] = L"MainWindow";
	

	

	HCURSOR hCursor = LoadCursor(thisInstance, IDC_ARROW);
	WNDCLASS wc = {};
	wc.lpfnWndProc = WindowProc;
	wc.lpszClassName = CLASSNAME;
	wc.hInstance = hInstance;
	wc.hCursor = hCursor;
	SetCursor(hCursor);

	RegisterClass(&wc);

	HWND hwnd = CreateWindowEx(0, CLASSNAME, L"Master of None", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	if (hwnd == NULL) {
		return 0;
	}
	ShowWindow(hwnd, nCmdShow);
	canvas.canvasHwnd = &hwnd;
	player.trackMovement();
	//camera->focusOn(&player.x,&player.y,&hwnd) ;
	MSG msg = {};
	while (GetMessage(&msg, hwnd, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_DESTROY: {
		pFactory->Release();
		PostQuitMessage(0);
		return 0;
	}
	case WM_CREATE: {
		if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory)  
		|| FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&pWriteFactory)))
		)) {
			return -1;
		}
		GetClientRect(hwnd, &clientWindow);
		//player.hwnd = &hwnd;
		//player.liveTrack(&fullWindow);

		return 0;
	}
	case WM_PAINT: {


		GetClientRect(hwnd, &clientWindow);
		GetWindowRect(hwnd, &fullWindow);

		canvas.initialize(&hwnd);
		canvas.SetBackground(color3.white);
		for (int i = 0; i <= Objects.size() - 1; ++i) {
			Objects[i]->draw();
			
		}

		
		canvas.destroy();
		return 0;
	}
	case WM_KEYDOWN: {
		wchar_t key = MapVirtualKeyA(wParam, MAPVK_VK_TO_CHAR);

		switch (wParam) {
		case VK_SPACE: {

			return 0;
		}
		case VK_RIGHT: {
			
			return 0;
		}
		case VK_LEFT: {
			
			return 0;
		}
		}

		return 0;
	}
	case WM_KEYUP: {
		switch (wParam) {
		case VK_RIGHT: {
			
			return 0;
		}
		case VK_LEFT: {
			
			return 0;
		}
		}
		return 0;
	}
	case WM_GETMINMAXINFO: {
		POINT stickySize = { windowMaxWidth,windowMaxHeight };
		MINMAXINFO* sizeInfo = (LPMINMAXINFO)lParam;
		sizeInfo->ptMaxTrackSize = stickySize;
		sizeInfo->ptMinTrackSize = stickySize;
	}
	case WM_MOUSEMOVE: {
		
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		POINT cursorPos;
		WINDOWINFO wInfo;
		GetWindowInfo(hwnd, &wInfo);
		float mouseX = GET_X_LPARAM(lParam);
		float mouseY = GET_Y_LPARAM(lParam);
		float mouseCursorX = mouseX + fullWindow.left;
		float mouseCursorY = mouseY + fullWindow.top;
		
		
		Entity* hitX = player.testMovex(mouseX - player.x);
		Entity* hitY = player.testMovey(mouseY - player.y);
		GetCursorPos(&cursorPos);

		if (
			(true/*hitX == nullptr && hitY == nullptr*/) /*|| (!hitX->canCollide || hitX == nullptr) && (!hitY->canCollide || hitY == nullptr)*/
			) {
			player.setXY(mouseX, mouseY);
		}
		else {
			SetCursorPos(fullWindow.left + player.x, fullWindow.top + player.y + 30);
			for (int i = 0; i < 100; ++i) {
				/*if (player.stuck()) {

				}*/
			}
			Entity* hitTarget=nullptr;
			if (hitX != nullptr) {
				hitTarget = hitX;
			}
			else if (hitY != nullptr) {
				hitTarget = hitY;
			}
			if (hitTarget->classID == L"Cage") {
				hitTarget->shake();
				
				
			}
			else if (hitTarget->classID == L"Prisoner") {
				//die
			}

		}
		return 0;
	}

	case WM_LBUTTONDOWN: {
		int cursorX = GET_X_LPARAM(lParam) - fullWindow.left;
		int cursorY = GET_Y_LPARAM(lParam) - fullWindow.top;

		Entity* hitTarget = player.getHitTarget();
		if (isEditing) {
			if (hitTarget != nullptr) {
				if (selected.object != hitTarget && selected.object != nullptr) { //undo changed to previous object
					selected.object->borderWidth = selected.originalBorderWidth;
					selected.object->borderColor = selected.originalBorderColor;
				}
				selected.object = hitTarget;
				selected.originalBorderWidth = hitTarget->borderWidth;
				selected.originalBorderColor = hitTarget->borderColor;
				hitTarget->borderColor = color3.skyblue;
				hitTarget->borderWidth = 4.0f;

			}
		}
		return 0;
	}
	}
	
	return DefWindowProc(hwnd, uMsg, wParam, lParam);

}






