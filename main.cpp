#include "main.h"
#include "rc.h"

int size_dialog::idd() const {
	return IDD_SIZE; 
}

bool size_dialog::on_init_dialog(){
	set_int(IDC_EDIT1, x_dialog);
	set_int(IDC_EDIT2, y_dialog);
	return true;
}

bool size_dialog::on_ok(){
	try {
		x_dialog = get_int(IDC_EDIT1);
		y_dialog = get_int(IDC_EDIT2);
	}
	catch (...) {
		return false;
	}
	return true;
}


void main_window::on_paint(HDC hdc){
	RECT rect;
	HBRUSH brush = CreateSolidBrush(my_color);

	GetClientRect(*this, &rect);
	SetMapMode(hdc, MM_ANISOTROPIC);
	SetViewportExtEx(hdc, rect.right, rect.bottom, NULL);
	SetWindowExtEx(hdc, x, y, NULL);

	for (int i = 0; i < x; i++) {
		for (int j = i % 2; j < y; j += 2) {
			RECT rc = { i, j, i + 1, j + 1 };
			FillRect(hdc, &rc, brush);
		}
	}
	DeleteObject(brush);

}

void main_window::on_command(int id){
	switch (id) {
		case ID_SIZE: {
			size_dialog size_dlg;
			size_dlg.x_dialog = x;
			size_dlg.y_dialog = y;

			if (size_dlg.do_modal(0, *this) == IDOK) {
				x = size_dlg.x_dialog;
				y = size_dlg.y_dialog;
				InvalidateRect(*this, NULL, true);
			}

			break;
		}
		case ID_COLOR: {
			COLORREF custom_colors[16] = {0};
			CHOOSECOLOR cc;

			ZeroMemory(&cc, sizeof cc);
			cc.lStructSize = sizeof cc;
			cc.Flags = CC_RGBINIT | CC_FULLOPEN;
			cc.lpCustColors = custom_colors;
			cc.hwndOwner = *this;
			cc.rgbResult = my_color;

			if (ChooseColor(&cc)) {
				my_color = cc.rgbResult;
				InvalidateRect(*this, NULL, true);
			}

			break;
		}
		case ID_EXIT: 
			DestroyWindow(*this); 
			break;
	}
}

void main_window::on_destroy(){
	::PostQuitMessage(0);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hp, LPSTR cmdLine, int nShow)
{
	vsite::nwp::application app;
	main_window wnd;	
	wnd.create(0, WS_OVERLAPPEDWINDOW | WS_VISIBLE, _T("NWP"), (int)LoadMenu(hInstance, MAKEINTRESOURCE(IDM_MAIN)));	
	return app.run();
}
