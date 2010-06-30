/*
Copyright (C) 2009-2010 wxLauncher Team

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include <wx/wx.h>
#include <wx/filename.h>
#include <wx/wfstream.h>
#include "generated/configure_launcher.h"
#include "apis/PlatformProfileManager.h"
#include "global/ids.h"

ProMan::RegistryCodes PushCmdlineFSO(wxFileConfig *cfg) {
	wxString modLine, flagLine, tcPath;
	cfg->Read(PRO_CFG_TC_CURRENT_MODLINE, &modLine);
	cfg->Read(PRO_CFG_TC_CURRENT_FLAG_LINE, &flagLine);
	cfg->Read(PRO_CFG_TC_ROOT_FOLDER, &tcPath);
	wxString cmdLineString;
	cmdLineString += tcPath.c_str();
	cmdLineString += wxFileName::GetPathSeparator();
	cmdLineString += _T("data");
	cmdLineString += wxFileName::GetPathSeparator();
	cmdLineString += _T("cmdline_fso.cfg");
	wxFileName cmdLineFileName(cmdLineString);
	wxFFileOutputStream outStream(cmdLineFileName.GetFullPath(), _T("w+b"));
	if ( !outStream.IsOk() ) {
		return ProMan::UnknownError;
	}
	if ( !modLine.IsEmpty()) {
		outStream.Write("-mod ", 5);
		outStream.Write(modLine.char_str(), modLine.size());
	}
	if ( !flagLine.IsEmpty() ) {
		outStream.Write(" ", 1);
		outStream.Write(flagLine.char_str(), flagLine.size());
	}
	if ( !outStream.Close() ) {
		return ProMan::UnknownError;
	}

	return ProMan::NoError;
}