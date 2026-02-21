#pragma once

#include <wx/wx.h>
#include <wx/taskbar.h>

class wxMTaskbar: public wxTaskBarIcon
{
    public:
        wxMTaskbar(wxFrame *_frame);

        virtual wxMenu* CreatePopupMenu() override;

    private:
        wxFrame *frame = nullptr;

        wxDECLARE_EVENT_TABLE();

        void OnDoubleClick(wxTaskBarIconEvent &evt);
        void OnMenuOpen(wxCommandEvent &evt);
        void OnMenuHide(wxCommandEvent &evt);
        void OnMenuQuit(wxCommandEvent &evt);
        void Delete();
};