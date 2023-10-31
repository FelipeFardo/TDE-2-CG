#include "Scene6.h"

CScene6::CScene6()
{
	pCamera = NULL;
	pTexto = NULL;
	pTextures = NULL;

	bIsWireframe = false;
	bIsCameraFPS = true;

	iFPS = 0;
	iFrames = 0;
	ulLastFPS = 0;
	szTitle[256] = 0;

	// Cria gerenciador de impress�o de texto na tela
	pTexto = new CTexto();

	// Cria camera
	pCamera = new CCamera(0.0f, 1.0f, 20.0f, 0.1f);

	// Cria o Timer
	pTimer = new CTimer();
	pTimer->Init();

	fTimerPosY = 0.0f;
	fRenderPosY = 0.0f;

	// Carrega todas as texturas
	pTextures = new CTexture();
	pTextures->CreateTextureMipMap(0, "../Scene1/CRATE.BMP");
	pTextures->CreateTextureMipMap(1, "../Scene1/grama.bmp");
	pTextures->CreateTextureMipMap(2, "../Scene1/parede.bmp");
	pTextures->CreateTextureMipMap(3, "../Scene1/paredeporta.bmp");
	pTextures->CreateTextureMipMap(4, "../Scene1/paredejanela.bmp");
	pTextures->CreateTextureMipMap(5, "../Scene1/R1.jpg");



	fRotY = 0.0f;

	fPosX = 0.0f;
	fPosY = 0.0f;
	fPosZ = 0.0f;


	for (int i = 0; i < 38; i++)
		faceColor[i] = { (rand() % 255), (rand() % 255), (rand() % 255) };

}


CScene6::~CScene6(void)
{
	if (pTexto)
	{
		delete pTexto;
		pTexto = NULL;
	}

	if (pTextures)
	{
		delete pTextures;
		pTextures = NULL;
	}

	if (pCamera)
	{
		delete pCamera;
		pCamera = NULL;
	}

	if (pTimer)
	{
		delete pTimer;
		pTimer = NULL;
	}
}




int CScene6::DrawGLScene(void)	// Fun��o que desenha a cena
{
	// Get FPS
	if (GetTickCount() - ulLastFPS >= 1000)	// When A Second Has Passed...
	{
		ulLastFPS = GetTickCount();				// Update Our Time Variable
		iFPS = iFrames;							// Save The FPS
		iFrames = 0;							// Reset The FPS Counter
	}
	iFrames++;									// FPS counter

	pTimer->Update();							// Atualiza o timer

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Limpa a tela e o Depth Buffer
	glLoadIdentity();									// Inicializa a Modelview Matrix Atual


	// Seta as posi��es da c�mera
	pCamera->setView();

	// Desenha grid 
	Draw3DSGrid(20.0f, 20.0f);

	// Desenha os eixos do sistema cartesiano
	DrawAxis();

	// Modo FILL ou WIREFRAME (pressione barra de espa�o)	
	if (bIsWireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//                               DESENHA OS OBJETOS DA CENA (IN�CIO)
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	
	pTextures->ApplyTexture(1);
	glPushMatrix();
	glTranslatef(0.0f, 0.01f, 0.0f);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-30.0f, 0.0f, -30.0f);
	glTexCoord2f(4.0f, 0.0f); glVertex3f(30.0f, 0.0f, -30.0f);
	glTexCoord2f(4.0f, 4.0f); glVertex3f(30.0f, 0.0f, 20.0f);
	glTexCoord2f(0.0f, 4.0f); glVertex3f(-30.0f, 0.0f, 30.0f);
	glEnd();
	glPopMatrix();

	// Caixas com textura
	DrawCrate(5.0f, 0.5f, 7.0f, 0.75f, 0.75f, 0.75f);
	DrawCrate(5.0f, 2.0f, 7.0f, 0.75f, 0.5f, 0.75f);
	DrawCrate(4.0f, 0.5f, 3.5f, 1.0f, 2.5f, 1.0f);

	// Hemps
	DrawHemps();


	glDisable(GL_TEXTURE_2D);
	glEnd();
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//                               DESENHA OS OBJETOS DA CENA (FIM)
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	fTimerPosY = pTimer->GetTime() / 1000.0f;
	fRenderPosY += 0.2f;

	// Impress�o de texto na tela...
	// Muda para modo de proje��o ortogonal 2D
	// OBS: Desabilite Texturas e Ilumina��o antes de entrar nesse modo de proje��o
	OrthoMode(0, 0, WIDTH, HEIGHT);


	glPushMatrix();
	glTranslatef(0.0f, HEIGHT - 150, 0.0f);	// Move 1 unidade para dentro da tela (eixo Z)

	// Cor da fonte
	glColor3f(1.0f, 1.0f, 0.0f);


	glRasterPos2f(10.0f, 0.0f);	// Posicionando o texto na tela
	if (!bIsWireframe) {
		pTexto->glPrint("[TAB]  Modo LINE"); // Imprime texto na tela
	}
	else {
		pTexto->glPrint("[TAB]  Modo FILL");
	}


	//// Camera LookAt
	glRasterPos2f(10.0f, 40.0f);
	pTexto->glPrint("Player LookAt  : %f, %f, %f", pCamera->Forward[0], pCamera->Forward[1], pCamera->Forward[2]);

	//// Posi��o do Player
	glRasterPos2f(10.0f, 60.0f);
	pTexto->glPrint("Player Position: %f, %f, %f", pCamera->Position[0], pCamera->Position[1], pCamera->Position[2]);

	//// Imprime o FPS da aplica��o e o Timer
	glRasterPos2f(10.0f, 80.0f);
	pTexto->glPrint("Frames-per-Second: %d ---- Timer: %.1f segundos", iFPS, (pTimer->GetTime() / 1000));

	glRasterPos2f(10.0f, 100.0f);
	pTexto->glPrint("fRot: %.1f ", fRotY);




	glPopMatrix();

	// Muda para modo de proje��o perspectiva 3D
	PerspectiveMode();

	return true;
}




void CScene6::MouseMove(void) // Tratamento de movimento do mouse
{
	// Realiza os c�lculos de rota��o da vis�o do Player (atrav�s das coordenadas
	// X do mouse.
	POINT mousePos;
	int middleX = WIDTH >> 1;
	int middleY = HEIGHT >> 1;

	GetCursorPos(&mousePos);

	if ((mousePos.x == middleX) && (mousePos.y == middleY)) return;

	SetCursorPos(middleX, middleY);

	fDeltaX = (float)((middleX - mousePos.x)) / 10;
	fDeltaY = (float)((middleY - mousePos.y)) / 10;

	// Rotaciona apenas a c�mera
	pCamera->rotateGlob(-fDeltaX, 0.0f, 1.0f, 0.0f);
	pCamera->rotateLoc(-fDeltaY, 1.0f, 0.0f, 0.0f);
}

void CScene6::KeyPressed(void) // Tratamento de teclas pressionadas
{

	// Verifica se a tecla 'W' foi pressionada e move o Player para frente
	if (GetKeyState('W') & 0x80)
	{
		pCamera->moveGlob(pCamera->Forward[0], pCamera->Forward[1], pCamera->Forward[2]);
	}
	// Verifica se a tecla 'S' foi pressionada e move o Player para tras
	else if (GetKeyState('S') & 0x80)
	{
		pCamera->moveGlob(-pCamera->Forward[0], -pCamera->Forward[1], -pCamera->Forward[2]);
	}
	// Verifica se a tecla 'A' foi pressionada e move o Player para esquerda
	else if (GetKeyState('A') & 0x80)
	{
		pCamera->moveGlob(-pCamera->Right[0], -pCamera->Right[1], -pCamera->Right[2]);
	}
	// Verifica se a tecla 'D' foi pressionada e move o Player para direira
	else if (GetKeyState('D') & 0x80)
	{
		pCamera->moveGlob(pCamera->Right[0], pCamera->Right[1], pCamera->Right[2]);
	}
	else if (GetKeyState('Q') & 0x80)
	{
		pCamera->moveGlob(0.0f, -pCamera->Up[1], 0.0f);
	}
	else if (GetKeyState('E') & 0x80)
	{
		pCamera->moveGlob(0.0f, pCamera->Up[1], 0.0f);
	}
	// Sen�o, interrompe movimento do Player
	else
	{
	}


	if (GetKeyState(VK_LEFT) & 0x80)
		fPosX -= 0.1f;
	if (GetKeyState(VK_RIGHT) & 0x80)
		fPosX += 0.1f;
	if (GetKeyState(VK_UP) & 0x80)
		fPosZ -= 0.1f;
	if (GetKeyState(VK_DOWN) & 0x80)
		fPosZ += 0.1f;
	if (GetKeyState(VK_PRIOR) & 0x80)
		fPosY += 0.1f;
	if (GetKeyState(VK_NEXT) & 0x80)
		fPosY -= 0.1f;


}

void CScene6::KeyDownPressed(WPARAM	wParam) // Tratamento de teclas pressionadas
{
	switch (wParam)
	{
	case VK_TAB:
		bIsWireframe = !bIsWireframe;
		break;

	case VK_SPACE:
		pTimer->Init();
		break;

	case VK_RETURN:
		break;

	}

}

//	Cria um grid horizontal ao longo dos eixos X e Z
void CScene6::Draw3DSGrid(float width, float length)
{

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(0.0f, 0.3f, 0.0f);
	glPushMatrix();
	for (float i = -width; i <= length; i += 1)
	{
		for (float j = -width; j <= length; j += 1)
		{
			// inicia o desenho das linhas
			glBegin(GL_QUADS);
			glNormal3f(0.0f, 1.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(i, 0.0f, j + 1);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(i + 1, 0.0f, j + 1);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(i + 1, 0.0f, j);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(i, 0.0f, j);
			glEnd();
		}
	}
	glPopMatrix();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}



void CScene6::DrawAxis()
{
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	// Eixo X
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-1000.0f, 0.0f, 0.0f);
	glVertex3f(1000.0f, 0.0f, 0.0f);

	// Eixo Y
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 1000.0f, 0.0f);
	glVertex3f(0.0f, -1000.0f, 0.0f);

	// Eixo Z
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 1000.0f);
	glVertex3f(0.0f, 0.0f, -1000.0f);
	glEnd();
	glPopMatrix();
}


void CScene6::DrawCrate(float x, float y, float z, float ScaleX, float ScaleY, float ScaleZ)
{
	glPushMatrix();
	pTextures->ApplyTexture(0);
	glScalef(ScaleX, ScaleY, ScaleZ);
	glTranslatef(x, y, z);
	glBegin(GL_QUADS);
	// face frente
	glTexCoord2d(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
	glTexCoord2d(1.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);
	glTexCoord2d(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, 0.5f);
	glTexCoord2d(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.5f);

	// face tr�s
	glTexCoord2d(0.0f, 0.0f); glVertex3f(0.5f, -0.5f, -0.5f);
	glTexCoord2d(0.0f, 1.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
	glTexCoord2d(1.0f, 1.0f); glVertex3f(-0.5f, 0.5f, -0.5f);
	glTexCoord2d(1.0f, 0.0f); glVertex3f(0.5f, 0.5f, -0.5f);

	// face direita
	glTexCoord2d(0.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);
	glTexCoord2d(0.0f, 1.0f); glVertex3f(0.5f, -0.5f, -0.5f);
	glTexCoord2d(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, -0.5f);
	glTexCoord2d(1.0f, 0.0f); glVertex3f(0.5f, 0.5f, 0.5f);

	// face esquerda
	glTexCoord2d(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
	glTexCoord2d(0.0f, 1.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
	glTexCoord2d(1.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.5f);
	glTexCoord2d(1.0f, 0.0f); glVertex3f(-0.5f, 0.5f, -0.5f);

	// face baixo
	glTexCoord2d(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
	glTexCoord2d(0.0f, 1.0f); glVertex3f(0.5f, -0.5f, -0.5f);
	glTexCoord2d(1.0f, 1.0f); glVertex3f(0.5f, -0.5f, 0.5f);
	glTexCoord2d(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);

	// face cima
	glTexCoord2d(0.0f, 0.0f); glVertex3f(-0.5f, 0.5f, 0.5f);
	glTexCoord2d(0.0f, 1.0f); glVertex3f(0.5f, 0.5f, 0.5f);
	glTexCoord2d(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, -0.5f);
	glTexCoord2d(1.0f, 0.0f); glVertex3f(-0.5f, 0.5f, -0.5f);

	glEnd();
	glPopMatrix();
}

void CScene6::DrawHemps() 
{
	// Janela frente
	pTextures->ApplyTexture(4); 
	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2d(0.0f, 0.0f); glVertex3f(-3.0f, 0.0f, 1.0f);
	glTexCoord2d(1.0f, 0.0f); glVertex3f(-1.0f, 0.0f, 1.0f);
	glTexCoord2d(1.0f, 1.0f); glVertex3f(-1.0f, 1.7f, 1.0f);
	glTexCoord2d(0.0f, 1.0f); glVertex3f(-3.0f, 1.7f, 1.0f);
	glEnd();
	glPopMatrix();

	// Porta frente
	pTextures->ApplyTexture(3);
	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2d(0.0f, 0.0f); glVertex3f(3.0f, 0.0f, 1.0f);
	glTexCoord2d(1.0f, 0.0f); glVertex3f(1.0f, 0.0f, 1.0f);
	glTexCoord2d(1.0f, 1.0f); glVertex3f(1.0f, 1.7f, 1.0f);
	glTexCoord2d(0.0f, 1.0f); glVertex3f(3.0f, 1.7f, 1.0f);
	glEnd();
	glPopMatrix();

	// Parede frente
	pTextures->ApplyTexture(2);
	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2d(0.0f, 0.0f); glVertex3f(-1.0f, 0.0f, 1.0f);
	glTexCoord2d(1.0f, 0.0f); glVertex3f(1.0f, 0.0f, 1.0f);
	glTexCoord2d(1.0f, 1.0f); glVertex3f(1.0f, 1.7f, 1.0f);
	glTexCoord2d(0.0f, 1.0f); glVertex3f(-1.0f, 1.7f, 1.0f);
	glEnd();
	glPopMatrix();

	// Janela tras
	pTextures->ApplyTexture(4);
	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2d(0.0f, 0.0f); glVertex3f(-3.0f, 0.0f, -1.0f);
	glTexCoord2d(1.0f, 0.0f); glVertex3f(-1.0f, 0.0f, -1.0f);
	glTexCoord2d(1.0f, 1.0f); glVertex3f(-1.0f, 1.7f, -1.0f);
	glTexCoord2d(0.0f, 1.0f); glVertex3f(-3.0f, 1.7f, -1.0f);
	glEnd();
	glPopMatrix();

	// Janela tras
	pTextures->ApplyTexture(4);
	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2d(0.0f, 0.0f); glVertex3f(3.0f, 0.0f, -1.0f);
	glTexCoord2d(1.0f, 0.0f); glVertex3f(1.0f, 0.0f, -1.0f);
	glTexCoord2d(1.0f, 1.0f); glVertex3f(1.0f, 1.7f, -1.0f);
	glTexCoord2d(0.0f, 1.0f); glVertex3f(3.0f, 1.7f, -1.0f);
	glEnd();
	glPopMatrix();

	// Parede tras
	pTextures->ApplyTexture(2);
	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2d(0.0f, 0.0f); glVertex3f(-1.0f, 0.0f, -1.0f);
	glTexCoord2d(1.0f, 0.0f); glVertex3f(1.0f, 0.0f, -1.0f);
	glTexCoord2d(1.0f, 1.0f); glVertex3f(1.0f, 1.7f, -1.0f);
	glTexCoord2d(0.0f, 1.0f); glVertex3f(-1.0f, 1.7f, -1.0f);
	glEnd();
	glPopMatrix();
	 
	// Janela lado 1
	pTextures->ApplyTexture(4);
	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2d(0.0f, 0.0f); glVertex3f(-3.0f, 0.0f, -1.0f);
	glTexCoord2d(1.0f, 0.0f); glVertex3f(-3.0f, 0.0f, 1.0f);
	glTexCoord2d(1.0f, 1.0f); glVertex3f(-3.0f, 1.7f, 1.0f);
	glTexCoord2d(0.0f, 1.0f); glVertex3f(-3.0f, 1.7f, -1.0f);
	glEnd();
	glPopMatrix();

	// Janela lado 2
	pTextures->ApplyTexture(4);
	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2d(0.0f, 0.0f); glVertex3f(3.0f, 0.0f, -1.0f);
	glTexCoord2d(1.0f, 0.0f); glVertex3f(3.0f, 0.0f, 1.0f);
	glTexCoord2d(1.0f, 1.0f); glVertex3f(3.0f, 1.7f, 1.0f);
	glTexCoord2d(0.0f, 1.0f); glVertex3f(3.0f, 1.7f, -1.0f);
	glEnd();
	glPopMatrix();

	
	// telhado frente
	pTextures->ApplyTexture(5);
	glBegin(GL_QUADS);
	glPushMatrix();
	glTexCoord2d(0.0f, 0.0f); glVertex3f(-3.1f, 1.0f, 1.90f);
	glTexCoord2d(1.0f, 0.0f); glVertex3f(3.1f, 1.0f, 1.90f);  
	glTexCoord2d(1.0f, 1.0f); glVertex3f(3.1f, 2.5f, 0.0f);
	glTexCoord2d(0.0f, 1.0f); glVertex3f(-3.1f, 2.5f, 0.0f); 
	glPopMatrix();

	
	// telhado tr�s
	glPushMatrix();
	glTexCoord2d(0.0f, 0.0f); glVertex3f(3.1f, 1.0f, -1.90f);  
	glTexCoord2d(1.0f, 0.0f); glVertex3f(-3.1f, 1.0f, -1.90f); 
	glTexCoord2d(1.0f, 1.0f); glVertex3f(-3.1f, 2.5f, 0.0f);  
	glTexCoord2d(0.0f, 1.0f); glVertex3f(3.1f, 2.5f, 0.0f);
	glPopMatrix();
	glEnd();

}

void CScene6::DrawPyramid()
{
	glBegin(GL_TRIANGLES);

	// Face frente
	glTexCoord2f(0.02f, 0.1f); glVertex3f(-0.5f, -0.5f, 0.5f);
	glTexCoord2f(0.435f, 0.1f); glVertex3f(0.5f, -0.5f, 0.5f);
	glTexCoord2f(0.23f, 0.45f); glVertex3f(0.0f, 0.5f, 0.0f);

	// Face direita
	glTexCoord2f(0.52f, 0.1f); glVertex3f(0.5f, -0.5f, 0.5f);
	glTexCoord2f(0.94f, 0.1f); glVertex3f(0.5f, -0.5f, -0.5f);
	glTexCoord2f(0.73f, 0.45f); glVertex3f(0.0f, 0.5f, 0.0f);

	// Face esquerda
	glTexCoord2f(0.02f, 0.56f); glVertex3f(0.5f, -0.5f, -0.5f);
	glTexCoord2f(0.435f, 0.56f); glVertex3f(-0.5f, -0.5f, -0.5f);
	glTexCoord2f(0.23f, 0.9f); glVertex3f(0.0f, 0.5f, 0.0f);

	// Face tr�s
	glTexCoord2f(0.53f, 0.56f); glVertex3f(-0.5f, -0.5f, -0.5f);
	glTexCoord2f(0.93f, 0.56f); glVertex3f(-0.5f, -0.5f, 0.5f);
	glTexCoord2f(0.73f, 0.9f); glVertex3f(0.0f, 0.5f, 0.0f);
	glEnd();
}