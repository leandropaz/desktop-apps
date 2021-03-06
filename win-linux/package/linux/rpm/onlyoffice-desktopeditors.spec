Summary: Desktop editors for text, spreadsheet and presentation files
Name: {{PACKAGE_NAME}}
Version: {{PRODUCT_VERSION}}
Release: {{BUILD_NUMBER}}
License: AGPLv3
Group: Applications/Office
URL: http://onlyoffice.com/
Vendor: Ascensio System SIA
Packager: Ascensio System SIA <support@onlyoffice.com>
#FullRequires: libX11, dejavu-lgc-sans-fonts, dejavu-lgc-sans-mono-fonts, dejavu-lgc-serif-fonts, dejavu-sans-fonts, dejavu-sans-mono-fonts, dejavu-serif-fonts, libreoffice-opensymbol-fonts, liberation-mono-fonts, liberation-sans-fonts, liberation-serif-fonts
Requires: libX11, libXScrnSaver, libcurl, gtkglext-libs, libstdc++ >= 4.8.0, boost-filesystem, xcb-util-renderutil, xcb-util-image, xcb-util-wm, libxcb, xcb-util-keysyms, dejavu-lgc-sans-fonts, dejavu-lgc-sans-mono-fonts, dejavu-lgc-serif-fonts, dejavu-sans-fonts, dejavu-sans-mono-fonts, dejavu-serif-fonts, libreoffice-opensymbol-fonts, liberation-mono-fonts, liberation-narrow-fonts, liberation-sans-fonts, liberation-serif-fonts
BuildArch: x86_64
AutoReq: no
AutoProv: no

%description
ONLYOFFICE DesktopEditors installation package
 ONLYOFFICE DesktopEditors is an application for editing office documents (text documents, spreadsheets and presentations) from onlyoffice cloud portal on local computer without browser using.

%prep
rm -rf "$RPM_BUILD_ROOT"

%build

%install

#install desktopeditor files
mkdir -p "$RPM_BUILD_ROOT"
cp -r ../../../common/onlyoffice/* "$RPM_BUILD_ROOT/"

%clean
rm -rf "$RPM_BUILD_ROOT"

%files
%attr(-, root, root) /opt/onlyoffice/desktopeditors/*
%attr(755, root, root) /usr/bin/*desktopeditors
%attr(-, root, root) /usr/share/applications/*.desktop
%pre

%post

set -e 		# fail on any error
set -u 		# treat unset variable as errors

# Add icons to the system icons
XDG_ICON_RESOURCE="`which xdg-icon-resource 2> /dev/null || true`"
if [ ! -x "$XDG_ICON_RESOURCE" ]; then
  echo "Error: Could not find xdg-icon-resource" >&2
  exit 1
fi
for icon in "/opt/onlyoffice/desktopeditors/asc-de-"*.png; do
  size="${icon##*/asc-de-}"
  "$XDG_ICON_RESOURCE" install --size "${size%.png}" "$icon" "asc-de"
done

UPDATE_MENUS="`which update-menus 2> /dev/null || true`"
if [ -x "$UPDATE_MENUS" ]; then
  update-menus
fi

# Update cache of .desktop file MIME types. Non-fatal since it's just a cache.
#update-desktop-database > /dev/null 2>&1 || true

/opt/onlyoffice/desktopeditors/post_install.sh

%preun

set -e

action="$1"
if [ "$2" = "in-favour" ]; then
  # Treat conflict remove as an upgrade.
  action="upgrade"
fi
# Don't clean-up just for an upgrade.`
if [ "$action" = "upgrade" ] ; then
  exit 0
fi

# Remove icons from the system icons
XDG_ICON_RESOURCE="`which xdg-icon-resource 2> /dev/null || true`"
if [ ! -x "$XDG_ICON_RESOURCE" ]; then
  echo "Error: Could not find xdg-icon-resource" >&2
  exit 1
fi
for icon in "/opt/onlyoffice/desktopeditors/asc-de-"*.png; do
  size="${icon##*/asc-de-}"
  "$XDG_ICON_RESOURCE" uninstall --size "${size%.png}" "asc-de"
done

UPDATE_MENUS="`which update-menus 2> /dev/null || true`"
if [ -x "$UPDATE_MENUS" ]; then
  update-menus
fi

# Update cache of .desktop file MIME types. Non-fatal since it's just a cache.
#update-desktop-database > /dev/null 2>&1 || true

%postun

set -e 		# fail on any error

