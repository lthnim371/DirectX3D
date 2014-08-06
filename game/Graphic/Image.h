#pragma once

////////////////////////////////////////////////////////////////////////////
// �̹����� �о���̰� �׸��� �۾��� ���ϵ��� ���� Ŭ����.

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
		// �̹��� ���ϸ����� �о���̰� �׸� �� ������ �κи� �׸����� �κе� ����.
		BOOL load(LPCTSTR szFileName, const Rect& rc);
		// �̹��� ���ϸ����� �̹����� �о���δ�.
		// �׸��� �κ��� �̹��� ��ü.
		BOOL load(LPCTSTR szFileName);
		void release();
		// �׸���..
		void Draw(HDC hdc);
		// ����ó���� �ϱ� ���� ����..
		void SetTransparent(COLORREF clr);
		// ���ĺ������� �׸��� ���� ����.
		void SetAlpha(BYTE alpha, bool _bSrcAlpha = false);
		// �׷��� �κ��� ũ�⸦ ��ȯ.
		Rect getRect();
		COLORREF getTransparentColor() const;
		void Move(const Rect& rc);
		void Move(const Point& pt);
		void Rotate(const float& angle);
		// �̹��� �ڵ� ��ü�� �ʿ�� �ϴ� ��쿡 ȣ���.
		operator HBITMAP ()
		{
			return hBitmap;
		}
		// getRect �� ���ϰ� ���� ���� ����...
		operator Rect ()
		{
			return rcSrc;
		}
	
	//�߰�
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