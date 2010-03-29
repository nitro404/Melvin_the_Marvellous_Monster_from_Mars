#include "Includes.h"

char * strtrimcpy(const char * data) {
	if(data == NULL) { return NULL; }
	char * newData;
	int length = strlen(data);
	if(length == 0) {
		newData = new char[1];
		*newData = '\0';
		return newData;
	}

	const char * head = data;
	const char * tail = data + (sizeof(char) * length) - 1;
	int startPos = 0, endPos = length - 1;
	while((*head == ' ' || *head == '\t') && startPos <= endPos) {
		head += sizeof(char);
		startPos++;
	}
	while((*tail == ' ' || *tail == '\t') && startPos <= endPos) {
		tail -= sizeof(char);
		endPos--;
	}
	if(startPos > endPos) {
		newData = new char[1];
		*newData = '\0';
		return newData;
	}

	newData = new char[endPos - startPos + 2];
	char * temp = newData;
	for(int i=startPos;i<=endPos;i++) {
		*temp = data[i];
		temp += sizeof(char);
	}
	*temp = '\0';
	return newData;
}

bool isTrue(const char * str) {
	return	(str != NULL && strlen(str) > 0) &&
			(str[0] == '1' ||
			 str[0] == 'y' ||
			 str[0] == 'Y' ||
			 str[0] == 't' ||
			 str[0] == 'T' ||
			 (strlen(str) > 1 && _stricmp(str, "on") == 0));
}

void quit(const char * title, const char * message, ...) {
	if(message != NULL) {
		char buffer[256];
		va_list parameters;
		va_start(parameters, message);
		vsprintf_s(buffer, 256, message, parameters);
		va_end(parameters);
		MessageBoxA(NULL, buffer, title, MB_OK);
	}
	exit(1);
}

void prompt(const char * message, ...) {
	if(message != NULL) {
		char buffer[256];
		va_list parameters;
		va_start(parameters, message);
		vsprintf_s(buffer, 256, message, parameters);
		va_end(parameters);
		MessageBoxA(NULL, buffer, "Message", MB_OK);
	}
}

#if _DEBUG

	struct testDrawPointVertex {
		float x, y, z, rhw;
		DWORD colour;
	};

	void testDrawPoint(LPDIRECT3DDEVICE9 d3dDevice, float x, float y) {
		float r = 2;
		testDrawPointVertex vertex[] = {
			{x-r, y-r, 0, 0.5, D3DCOLOR_XRGB(0, 255, 0)},
			{x+r, y-r, 0, 0.5, D3DCOLOR_XRGB(0, 255, 0)},
			{x+r, y+r, 0, 0.5, D3DCOLOR_XRGB(0, 255, 0)},
			{x-r, y+r, 0, 0.5, D3DCOLOR_XRGB(0, 255, 0)},
			{x-r, y-r, 0, 0.5, D3DCOLOR_XRGB(0, 255, 0)}
		};

		d3dDevice->SetFVF(D3DFVF_XYZRHW|D3DFVF_DIFFUSE);
		d3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 4, &vertex, sizeof(testDrawPointVertex));
	}

	void testDrawEmptyBox(LPDIRECT3DDEVICE9 d3dDevice, float x, float y, float r) {
		testDrawPointVertex vertex[] = {
			{x-r, y-r, 0, 0.5, D3DCOLOR_XRGB(0, 255, 0)},
			{x+r, y-r, 0, 0.5, D3DCOLOR_XRGB(0, 255, 0)},
			{x+r, y+r, 0, 0.5, D3DCOLOR_XRGB(0, 255, 0)},
			{x-r, y+r, 0, 0.5, D3DCOLOR_XRGB(0, 255, 0)},
			{x-r, y-r, 0, 0.5, D3DCOLOR_XRGB(0, 255, 0)}
		};

		d3dDevice->SetFVF(D3DFVF_XYZRHW|D3DFVF_DIFFUSE);
		d3dDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, 4, &vertex, sizeof(testDrawPointVertex));
	}

	void testDrawEmptyBox(LPDIRECT3DDEVICE9 d3dDevice, float x, float y, float xr, float yr) {
		testDrawPointVertex vertex[] = {
			{x-xr, y-yr, 0, 0.5, D3DCOLOR_XRGB(0, 255, 0)},
			{x+xr, y-yr, 0, 0.5, D3DCOLOR_XRGB(0, 255, 0)},
			{x+xr, y+yr, 0, 0.5, D3DCOLOR_XRGB(0, 255, 0)},
			{x-xr, y+yr, 0, 0.5, D3DCOLOR_XRGB(0, 255, 0)},
			{x-xr, y-yr, 0, 0.5, D3DCOLOR_XRGB(0, 255, 0)}
		};

		d3dDevice->SetFVF(D3DFVF_XYZRHW|D3DFVF_DIFFUSE);
		d3dDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, 4, &vertex, sizeof(testDrawPointVertex));
	}

	void testDrawEmptyCircle(LPDIRECT3DDEVICE9 d3dDevice, float x, float y, float xr, float yr) {
		float c = 16;
		float a = 0;
		float i = 360.0f / c;
		testDrawPointVertex vertex[] = {
			{x-(sin(D3DXToRadian(a+=i))*xr), y-(cos(D3DXToRadian(a))*yr), 0, 0.5, D3DCOLOR_XRGB(0, 255, 0)},
			{x-(sin(D3DXToRadian(a+=i))*xr), y-(cos(D3DXToRadian(a))*yr), 0, 0.5, D3DCOLOR_XRGB(0, 255, 0)},
			{x-(sin(D3DXToRadian(a+=i))*xr), y-(cos(D3DXToRadian(a))*yr), 0, 0.5, D3DCOLOR_XRGB(0, 255, 0)},
			{x-(sin(D3DXToRadian(a+=i))*xr), y-(cos(D3DXToRadian(a))*yr), 0, 0.5, D3DCOLOR_XRGB(0, 255, 0)},
			{x-(sin(D3DXToRadian(a+=i))*xr), y-(cos(D3DXToRadian(a))*yr), 0, 0.5, D3DCOLOR_XRGB(0, 255, 0)},
			{x-(sin(D3DXToRadian(a+=i))*xr), y-(cos(D3DXToRadian(a))*yr), 0, 0.5, D3DCOLOR_XRGB(0, 255, 0)},
			{x-(sin(D3DXToRadian(a+=i))*xr), y-(cos(D3DXToRadian(a))*yr), 0, 0.5, D3DCOLOR_XRGB(0, 255, 0)},
			{x-(sin(D3DXToRadian(a+=i))*xr), y-(cos(D3DXToRadian(a))*yr), 0, 0.5, D3DCOLOR_XRGB(0, 255, 0)},
			{x-(sin(D3DXToRadian(a+=i))*xr), y-(cos(D3DXToRadian(a))*yr), 0, 0.5, D3DCOLOR_XRGB(0, 255, 0)},
			{x-(sin(D3DXToRadian(a+=i))*xr), y-(cos(D3DXToRadian(a))*yr), 0, 0.5, D3DCOLOR_XRGB(0, 255, 0)},
			{x-(sin(D3DXToRadian(a+=i))*xr), y-(cos(D3DXToRadian(a))*yr), 0, 0.5, D3DCOLOR_XRGB(0, 255, 0)},
			{x-(sin(D3DXToRadian(a+=i))*xr), y-(cos(D3DXToRadian(a))*yr), 0, 0.5, D3DCOLOR_XRGB(0, 255, 0)},
			{x-(sin(D3DXToRadian(a+=i))*xr), y-(cos(D3DXToRadian(a))*yr), 0, 0.5, D3DCOLOR_XRGB(0, 255, 0)},
			{x-(sin(D3DXToRadian(a+=i))*xr), y-(cos(D3DXToRadian(a))*yr), 0, 0.5, D3DCOLOR_XRGB(0, 255, 0)},
			{x-(sin(D3DXToRadian(a+=i))*xr), y-(cos(D3DXToRadian(a))*yr), 0, 0.5, D3DCOLOR_XRGB(0, 255, 0)},
			{x-(sin(D3DXToRadian(a+=i))*xr), y-(cos(D3DXToRadian(a))*yr), 0, 0.5, D3DCOLOR_XRGB(0, 255, 0)},
			{x-(sin(D3DXToRadian(a+=i))*xr), y-(cos(D3DXToRadian(a))*yr), 0, 0.5, D3DCOLOR_XRGB(0, 255, 0)}
		};

		d3dDevice->SetFVF(D3DFVF_XYZRHW|D3DFVF_DIFFUSE);
		d3dDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, (unsigned int) (c), &vertex, sizeof(testDrawPointVertex));
	}

#endif
