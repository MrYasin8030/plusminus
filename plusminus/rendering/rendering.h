
#include <d2d1.h>
#include <dwrite_1.h>
#include <intrin.h>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define RET_CHK(x) if ( x != S_OK ) return

namespace Renderer {
	ID2D1Factory* Interface;
	ID2D1RenderTarget* Canvas;
	IDWriteFactory1* TextEngine;
	IDWriteTextFormat* TextFormat;
	IDWriteTextFormat* BigText;
	ID2D1SolidColorBrush* SolidColor;
	bool initialized = false;

	UINT wcslen(const wchar_t* Str) {
		const wchar_t* TempStr = Str;
		for (; *TempStr; ++TempStr);
		return (UINT)(TempStr - Str);
	}
	bool InitRender(IDXGISwapChain* SwapChain) {
		if (!initialized) {
			initialized = true;
			D2D1_FACTORY_OPTIONS CreateOpt = {D2D1_DEBUG_LEVEL_NONE};
			DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(TextEngine), (IUnknown**)&TextEngine);
			D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory), &CreateOpt, (void**)&Interface);
			TextEngine->CreateTextFormat(xorstr(L"Consola"), NULL, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 12.f, L"", &TextFormat);
			if (!Interface || !TextEngine || !TextFormat) return false;
		}
		ID3D11Device* d3d_device;
		if (SwapChain->GetDevice(IID_PPV_ARGS(&d3d_device))) return false;
		WORD flagsOffset = *(WORD*)((*(DWORD64**)d3d_device)[ 38 ] + 2); //x64
		int& flags = *(INT*)((DWORD64)d3d_device + flagsOffset);
		d3d_device->Release( );
		IDXGISurface* d3d_bbuf;
		if (SwapChain->GetBuffer(0, IID_PPV_ARGS(&d3d_bbuf)))
			return false;
		D2D1_RENDER_TARGET_PROPERTIES d2d_prop = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_HARDWARE, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));
		flags |= 0x20; HRESULT canvas_state = Interface->CreateDxgiSurfaceRenderTarget(d3d_bbuf, d2d_prop, &Canvas); flags &= ~0x20; d3d_bbuf->Release( ); if (canvas_state) return false;
		if (!SolidColor) Canvas->CreateSolidColorBrush({}, &SolidColor); return true;
	}
	bool NewFrame(IDXGISwapChain* SwapChain) {
		if (!Canvas && !InitRender(SwapChain))
			return false;
		Canvas->BeginDraw( );
		return true;
	}
	Vector2 CanvasSize( ) {
		D2D1_SIZE_F Size = Canvas->GetSize( );
		return Vector2{Size.width, Size.height};
	}
	void ResetCanvas( ) {
		if (Canvas) {
			Canvas->Release( );
			Canvas = nullptr;
		}
	}
	void EndFrame( ) {
		HRESULT state = Canvas->EndDraw( );
		if (state == D2DERR_RECREATE_TARGET)
			ResetCanvas( );
	}
	void Line(const Vector2& Start, const Vector2& End, const D2D1::ColorF& Clr, float Thick = 1.5f, bool outline = false) {
		if (outline) {
			SolidColor->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
			Canvas->DrawLine({Start.x, Start.y}, {End.x, End.y}, SolidColor, Thick * 1.75);
		}
		SolidColor->SetColor(Clr);
		Canvas->DrawLine({Start.x, Start.y}, {End.x, End.y}, SolidColor, Thick);
	}
	void Circle(const Vector2& Start, const D2D1::ColorF& Clr, float Rad, float Thick = 1.5f) {
		SolidColor->SetColor(Clr);
		Canvas->DrawEllipse({{ Start.x, Start.y }, Rad, Rad}, SolidColor, Thick);
	}
	void FillCircle(const Vector2& Start, const D2D1::ColorF& Clr, float Rad) {
		SolidColor->SetColor(Clr);
		Canvas->FillEllipse({{ Start.x, Start.y }, Rad, Rad}, SolidColor);
	}
	void Rectangle(const Vector2& Start, const Vector2& Sz, const D2D1::ColorF& Clr, float Thick = 1.5f) {
		SolidColor->SetColor(Clr);
		Canvas->DrawRectangle({Start.x, Start.y, Start.x + Sz.x, Start.y + Sz.y}, SolidColor, Thick);
	}
	void FillRectangle(const Vector2& Start, const Vector2& Sz, const D2D1::ColorF& Clr) {
		SolidColor->SetColor(Clr);
		Canvas->FillRectangle({Start.x, Start.y, Start.x + Sz.x, Start.y + Sz.y}, SolidColor);
	}
	void OutlineRect(const Vector2& pos, const Vector2& size, const D2D1::ColorF& color, const D2D1::ColorF& oColor, float thickness) {
		SolidColor->SetColor(color);
		Canvas->FillRectangle({pos.x, pos.y, pos.x + size.x, pos.y + size.y}, SolidColor);
		SolidColor->SetColor(oColor);
		Canvas->DrawRectangle({pos.x, pos.y, pos.x + size.x, pos.y + size.y}, SolidColor, thickness);
	}
	void RoundedRectangle(const Vector2& Start, const Vector2& Sz, const D2D1::ColorF& Clr, float Rad, float Thick = 1.5f) {
		SolidColor->SetColor(Clr);
		Canvas->DrawRoundedRectangle({{Start.x, Start.y, Start.x + Sz.x, Start.y + Sz.y }, Rad, Rad}, SolidColor, Thick);
	}
	void FillRoundedRectangle(const Vector2& Start, const Vector2& Sz, const D2D1::ColorF& Clr, float Rad) {
		SolidColor->SetColor(Clr);
		Canvas->FillRoundedRectangle({{Start.x, Start.y, Start.x + Sz.x, Start.y + Sz.y}, Rad, Rad}, SolidColor);
	}
	void RectanglePoint(const Vector2 middle, float sz, const D2D1::ColorF clr) {
		Canvas->DrawRectangle({middle.x - (sz / 2), middle.y - (sz / 2), middle.x + sz, middle.y + sz}, SolidColor, 1.f);
	}
	void Triangle(const Vector2 top, const D2D1::ColorF clr, float sz) {
		Line(top, top + Vector2(sz, sz), clr);
		Line(top, top - Vector2(sz, -sz), clr);
		Line(top + Vector2(sz, sz), top - Vector2(sz, -sz), clr);
	}
	Vector2 CosTanSinLineH(float flAngle, float range, int x, int y, int LineLength) {
		float nigga = flAngle;
		nigga += 45.f;

		float flYaw = (nigga) * (PI / 180.0);

		float viewcosyawzzzzzzz = cos(flYaw);
		float viewsinyawzzzzzzz = sin(flYaw);

		float x2 = range * (-viewcosyawzzzzzzz) + range * viewsinyawzzzzzzz;
		float y2 = range * (-viewcosyawzzzzzzz) - range * viewsinyawzzzzzzz;

		int posonscreenX = x + int(x2 / range * (LineLength));
		int posonscreenY = y + int(y2 / range * (LineLength));
		
		return Vector2(posonscreenX, posonscreenY);
	}
	void CosTanSinLine(float flAngle, float range, int x, int y, int LineLength, const D2D1::ColorF& Clr, bool oof = false, bool outline = false) {
		float nigga = flAngle;
		nigga += 45.f;

		float flYaw = (nigga) * (PI / 180.0);

		float viewcosyawzzzzzzz = cos(flYaw);
		float viewsinyawzzzzzzz = sin(flYaw);

		float x2 = range * (-viewcosyawzzzzzzz) + range * viewsinyawzzzzzzz;
		float y2 = range * (-viewcosyawzzzzzzz) - range * viewsinyawzzzzzzz;

		int posonscreenX = x + int(x2 / range * (LineLength));
		int posonscreenY = y + int(y2 / range * (LineLength));

		if (!oof) {
			Line(Vector2(x, y), Vector2(posonscreenX, posonscreenY), Clr);
		}
		else {
			Line(CosTanSinLineH(flAngle, range, x, y, LineLength / 1.1), Vector2(posonscreenX, posonscreenY), Clr);
			if (outline) {
				Line(CosTanSinLineH(flAngle, range, x, y, LineLength / 1.1), Vector2(posonscreenX, posonscreenY), Clr, 1.5f, true);
			}
		}
		FillCircle(Vector2(posonscreenX, posonscreenY), Clr, 4);//P.S this is the small dot at the end of each line!!!!!!!!!XD
		if (outline) {
			FillCircle(Vector2(posonscreenX, posonscreenY), D2D1::ColorF::Black, vars::stuff::testFloat);
		}
	}
	template <typename ...Args>
	void Text(const Vector2 pos, const D2D1::ColorF clr, bool center, bool outline, const std::wstring_view text, Args&&... args) {
		const auto size = static_cast<std::size_t>(std::swprintf(nullptr, 0, text.data( ), std::forward<Args>(args)...) + 1);

		const std::unique_ptr<wchar_t [ ]> buffer(new wchar_t[ size ]);
		std::swprintf(buffer.get( ), size, text.data( ), std::forward<Args>(args)...);

		const auto str = std::wstring(buffer.get( ), buffer.get( ) + size - 1);
		const auto str_len = static_cast<std::uint32_t>(str.size( ));

		IDWriteTextLayout* dwrite_layout = nullptr;
		RET_CHK(TextEngine->CreateTextLayout(str.c_str( ), str_len, TextFormat, vars::stuff::ScreenWidth, vars::stuff::ScreenHeight, &dwrite_layout));

		const DWRITE_TEXT_RANGE range
		{
			0,
			str_len
		};

		dwrite_layout->SetFontSize(12.f, range);

		if (center) {
			SolidColor->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
			DWRITE_TEXT_METRICS TextInfo;
			dwrite_layout->GetMetrics(&TextInfo);
			Vector2 TextSize = {TextInfo.width / 2.f, TextInfo.height / 2.f};
			const auto x = pos.x - TextSize.x;
			const auto y = pos.y - TextSize.y;
			if (outline) {

				Canvas->DrawTextLayout(D2D1::Point2F(x - 1, y), dwrite_layout, SolidColor);
				Canvas->DrawTextLayout(D2D1::Point2F(x + 1, y), dwrite_layout, SolidColor);
				Canvas->DrawTextLayout(D2D1::Point2F(x, y - 1), dwrite_layout, SolidColor);
				Canvas->DrawTextLayout(D2D1::Point2F(x, y + 1), dwrite_layout, SolidColor);
			}

			SolidColor->SetColor(clr);

			Canvas->DrawTextLayout(D2D1::Point2F(x, y), dwrite_layout, SolidColor);
			dwrite_layout->Release( );
			return;
		}


		SolidColor->SetColor(D2D1::ColorF(D2D1::ColorF::Black));

		if (outline) {
			const auto x = pos.x;
			const auto y = pos.y;

			Canvas->DrawTextLayout(D2D1::Point2F(x - 1, y), dwrite_layout, SolidColor);
			Canvas->DrawTextLayout(D2D1::Point2F(x + 1, y), dwrite_layout, SolidColor);
			Canvas->DrawTextLayout(D2D1::Point2F(x, y - 1), dwrite_layout, SolidColor);
			Canvas->DrawTextLayout(D2D1::Point2F(x, y + 1), dwrite_layout, SolidColor);
		}

		SolidColor->SetColor(clr);
		Canvas->DrawTextLayout(D2D1::Point2F(pos.x, pos.y), dwrite_layout, SolidColor);
		dwrite_layout->Release( );
	}
}
