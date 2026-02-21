#include "Taskbar.hpp"

wxBEGIN_EVENT_TABLE(wxMTaskbar, wxTaskBarIcon)
    EVT_TASKBAR_LEFT_DCLICK(wxMTaskbar::OnDoubleClick)
    EVT_MENU(wxID_OPEN, wxMTaskbar::OnMenuOpen)
    EVT_MENU(wxID_CLOSE, wxMTaskbar::OnMenuHide)
    EVT_MENU(wxID_EXIT, wxMTaskbar::OnMenuQuit)
wxEND_EVENT_TABLE()

wxMTaskbar::wxMTaskbar(wxFrame *_frame) : frame(_frame) { }

void wxMTaskbar::OnDoubleClick(wxTaskBarIconEvent &evt)
{
    frame->Iconize(false);
    frame->SetFocus();
    frame->Raise();
    frame->Show();

    evt.Skip();
}

wxMenu* wxMTaskbar::CreatePopupMenu()
{
    wxMenu *menu = new wxMenu;
    menu->Append(wxID_OPEN, "Open");
    menu->Append(wxID_CLOSE, "Hide");
    menu->AppendSeparator();
    menu->Append(wxID_EXIT, "Quit");
    return menu;
}

void wxMTaskbar::OnMenuOpen(wxCommandEvent &evt)
{
    frame->Iconize(false);
    frame->SetFocus();
    frame->Raise();
    frame->Show();
}

void wxMTaskbar::OnMenuHide(wxCommandEvent &evt)
{
    frame->Iconize(false);
    frame->SetFocus();
    frame->Raise();
    frame->Hide();
}

void wxMTaskbar::OnMenuQuit(wxCommandEvent &evt)
{
    // Force close — bypasses the hide-to-tray behaviour so the app actually exits
    frame->Close(true);
}

void wxMTaskbar::Delete()
{
    delete frame;
}