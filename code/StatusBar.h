#ifndef STATUSLOG_H
#define STATUSLOG_H

#include <wx/wx.h>

enum StatusLogIDS: int {
	ID_STATUS_TOTAL_PROGRESS = 1,
	ID_STATUS_TASK_PROGRESS,
	ID_INFO_PANEL,
};

class StatusLog {
public:
	StatusLog();

	wxPanel* GetGaugePanel(wxWindow* parent);
	wxPanel* GetInfoPanel(wxWindow* parent);
	bool IsOk();



protected:
	class StatusLogInfoPanel: public wxPanel {
	public:
		StatusLogInfoPanel(wxWindow* parent);

		void OnPaint(wxPaintEvent &event);
		void MouseEnter(wxMouseEvent &event);
		void MouseExit(wxMouseEvent &event);
		void MouseUp(wxMouseEvent &event);
		void MouseDown(wxMouseEvent &event);

		void OpenLogWindow();

	protected:
		void SetMessage(wxString &msg);
	private:
		wxString* currentMessage;
		wxFont *enabledFont, *disabledFont, *hoverFont;
		wxColour *enabledColour, *disabledColour;
		bool mouseHovering, mouseClicking;

		DECLARE_EVENT_TABLE();
	};

private:
	wxPanel *gaugePanel;
	StatusLogInfoPanel *infoPanel;
	wxGauge *totalProgress, *taskProgress;

	wxCriticalSection *taskProgressLock;	//!< lock held when making changes to the progress bar.
	wxCriticalSection *messageLock;			//!< lock held when changing the currentMessage.
};

#endif