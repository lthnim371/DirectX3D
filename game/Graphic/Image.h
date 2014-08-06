#pragma once

////////////////////////////////////////////////////////////////////////////
// 이미지를 읽어들이고 그리는 작업을 편리하도록 만든 클래스.

#define D2R 0.0174532f // PI/180

namespace graphic
{
	class cImage
	{
	public :
		cImage();
		~cImage();

	public :
		cImage* clone();
		// 이미지 파일명으로 읽어들이고 그릴 때 지정된 부분만 그리도록 부분도 설정.
		BOOL load(LPCTSTR szFileName, const Rect& rc);
		// 이미지 파일명만으로 이미지를 읽어들인다.
		// 그리는 부분은 이미지 전체.
		BOOL load(LPCTSTR szFileName);
		void release();
		// 그리기..
		void Draw(HDC hdc);
		// 투명처리를 하기 위한 설정..
		void SetTransparent(COLORREF clr);
		// 알파블랜딩으로 그리기 위한 설정.
		void SetAlpha(BYTE alpha, bool _bSrcAlpha = false);
		// 그려질 부분의 크기를 반환.
		Rect getRect();
		COLORREF getTransparentColor() const;
		void Move(const Rect& rc);
		void Move(const Point& pt);
		void Rotate(const float& angle);
		// 이미지 핸들 자체를 필요로 하는 경우에 호출됨.
		operator HBITMAP ()
		{
			return hBitmap;
		}
		// getRect 를 편하게 쓰기 위해 선언...
		operator Rect ()
		{
			return rcSrc;
		}
	
	//추가
		void SetSourRect(const Rect& rc);

	private :
		void _checkSupportedFile(LPCTSTR lpszFileName);

	private :
		HBITMAP hBitmap;
		Rect rcDest;
		Rect rcSrc;

		COLORREF clrTransparent;
		bool bUseTransparent;

		///////////////////////////
		// alpha
		bool bUseAlpha;
		BYTE byteAlpha;
		bool bSrcAlpha;

		//////////////////////////
		// rotate
		bool bUseRotate;
		float fAngle;
	};
}