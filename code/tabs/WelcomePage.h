#ifndef WELCOMEPAGE_H
#define WELCOMEPAGE_H

#include <wx/wx.h>

#include "apis/SkinManager.h"
#include "global/ids.h"
#include "apis/ProfileManager.h"

class WelcomePage: public wxWindow {
public:
	WelcomePage(wxWindow* parent, SkinSystem* skin);
	void LinkClicked(wxHtmlLinkEvent& event);
	void LinkHover(wxHtmlCellEvent& event);
	void OnMouseOut(wxMouseEvent& event);
	void ProfileButtonClicked(wxCommandEvent& event);
	void SaveDefaultChecked(wxCommandEvent& event);
	void ProfileChanged(wxCommandEvent& event);
	void ProfileCountChanged(wxCommandEvent& event);
	void UpdateNews(wxIdleEvent& event);
	void OnDownloadNewsCheck(wxCommandEvent& event);
	void OnUpdateNewsHelp(wxCommandEvent& event);
	
private:
	/** The width of the items on the welcome tab. */
	const static int stuffWidth = TAB_AREA_WIDTH;
	void *lastLinkInfo;

	void cloneNewProfile(wxChoice* combobox, ProMan* profile);
	void deleteProfile(wxChoice* combobox, ProMan* profile);

	bool needToUpdateNews;
	bool getOrPromptUpdateNews();

	DECLARE_EVENT_TABLE();
};

#endif