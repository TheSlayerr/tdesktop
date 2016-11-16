/*
This file is part of Telegram Desktop,
the official desktop version of Telegram messaging app, see https://telegram.org

Telegram Desktop is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

It is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

In addition, as a special exception, the copyright holders give permission
to link the code of portions of this program with the OpenSSL library.

Full license: https://github.com/telegramdesktop/tdesktop/blob/master/LICENSE
Copyright (c) 2014-2016 John Preston, https://desktop.telegram.org
*/
#pragma once

#include "boxes/abstractbox.h"
#include "core/observer.h"

namespace Ui {
class Radiobutton;
class LinkButton;
class RoundButton;
} // namespace Ui

class DownloadPathBox : public AbstractBox {
	Q_OBJECT

public:
	DownloadPathBox();

public slots:
	void onChange();
	void onEditPath();
	void onSave();

protected:
	void paintEvent(QPaintEvent *e) override;
	void resizeEvent(QResizeEvent *e) override;

	void showAll() override;

private:
	void setPathText(const QString &text);

	QString _path;
	QByteArray _pathBookmark;

	ChildWidget<Ui::Radiobutton> _default;
	ChildWidget<Ui::Radiobutton> _temp;
	ChildWidget<Ui::Radiobutton> _dir;
	ChildWidget<Ui::LinkButton> _pathLink;

	ChildWidget<Ui::RoundButton> _save;
	ChildWidget<Ui::RoundButton> _cancel;

};
