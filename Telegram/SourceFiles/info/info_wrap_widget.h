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
Copyright (c) 2014-2017 John Preston, https://desktop.telegram.org
*/
#pragma once

#include <rpl/variable.h>
#include <rpl/event_stream.h>
#include "window/section_widget.h"

namespace Ui {
class SettingsSlider;
class FadeShadow;
class PlainShadow;
class DropdownMenu;
class IconButton;
} // namespace Ui

namespace Window {
enum class SlideDirection;
} // namespace Window

namespace Info {
namespace Profile {
class Widget;
} // namespace Profile

namespace Media {
class Widget;
} // namespace Media

class Controller;
class Section;
class Memento;
class MoveMemento;
class ContentMemento;
class ContentWidget;
class TopBar;

enum class Wrap {
	Layer,
	Narrow,
	Side,
};

struct SelectedItem {
	explicit SelectedItem(FullMsgId msgId) : msgId(msgId) {
	}

	FullMsgId msgId;
	bool canDelete = false;
	bool canForward = false;
};

struct SelectedItems {
	explicit SelectedItems(Storage::SharedMediaType type)
	: type(type) {
	}

	Storage::SharedMediaType type;
	std::vector<SelectedItem> list;

};

class WrapWidget final : public Window::SectionWidget {
public:
	WrapWidget(
		QWidget *parent,
		not_null<Window::Controller*> window,
		Wrap wrap,
		not_null<Memento*> memento);

	not_null<PeerData*> peer() const;
	PeerData *activePeer() const override {
		return peer();
	}
	Wrap wrap() const {
		return _wrap.current();
	}
	rpl::producer<Wrap> wrapValue() const;
	void setWrap(Wrap wrap);

	not_null<Controller*> controller() {
		return _controller.get();
	}

	bool hasTopBarShadow() const override;
	QPixmap grabForShowAnimation(
		const Window::SectionSlideParams &params) override;

	void forceContentRepaint();

	bool showInternal(
		not_null<Window::SectionMemento*> memento,
		const Window::SectionShow &params) override;
	std::unique_ptr<Window::SectionMemento> createMemento() override;

	rpl::producer<int> desiredHeightValue() const override;

	void updateInternalState(not_null<Memento*> memento);

	// Float player interface.
	bool wheelEventFromFloatPlayer(QEvent *e) override;
	QRect rectForFloatPlayer() const override;

	object_ptr<Ui::RpWidget> createTopBarSurrogate(QWidget *parent);

	void updateGeometry(QRect newGeometry, int additionalScroll);
	int scrollTillBottom(int forHeight) const;
	rpl::producer<int>  scrollTillBottomChanges() const;

	~WrapWidget();

protected:
	void resizeEvent(QResizeEvent *e) override;
	void keyPressEvent(QKeyEvent *e) override;

	void doSetInnerFocus() override;
	void showFinishedHook() override;

	void showAnimatedHook(
		const Window::SectionSlideParams &params) override;

private:
	using SlideDirection = Window::SlideDirection;
	using SectionSlideParams = Window::SectionSlideParams;
	//enum class Tab {
	//	Profile,
	//	Media,
	//	None,
	//};
	struct StackItem;

	void startInjectingActivePeerProfiles();
	void injectActivePeerProfile(not_null<PeerData*> peer);
	void restoreHistoryStack(
		std::vector<std::unique_ptr<ContentMemento>> stack);
	bool hasStackHistory() const {
		return !_historyStack.empty();
	}
	void showBackFromStack();
	void showNewContent(not_null<ContentMemento*> memento);
	void showNewContent(
		not_null<ContentMemento*> memento,
		const Window::SectionShow &params);
	bool returnToFirstStackFrame(
		not_null<ContentMemento*> memento,
		const Window::SectionShow &params);
	void setupTop();
	//void setupTabbedTop();
	//void setupTabs(Tab tab);
	//void createTabs();
	void createTopBar();
	void highlightTopBar();

	not_null<RpWidget*> topWidget() const;

	QRect contentGeometry() const;
	rpl::producer<int> desiredHeightForContent() const;
	void finishShowContent();
	rpl::producer<bool> topShadowToggledValue() const;
	void updateContentGeometry();

	//void showTab(Tab tab);
	void showContent(object_ptr<ContentWidget> content);
	//std::unique_ptr<ContentMemento> createTabMemento(Tab tab);
	object_ptr<ContentWidget> createContent(
		not_null<ContentMemento*> memento,
		not_null<Controller*> controller);
	std::unique_ptr<Controller> createController(
		not_null<Window::Controller*> window,
		not_null<ContentMemento*> memento);
	//void convertProfileFromStackToTab();

	rpl::producer<SelectedItems> selectedListValue() const;
	bool requireTopBarSearch() const;

	void addProfileMenuButton();
	void addProfileNotificationsButton();
	void showProfileMenu();

	rpl::variable<Wrap> _wrap;
	std::unique_ptr<Controller> _controller;
	object_ptr<ContentWidget> _content = { nullptr };
	int _additionalScroll = 0;
	//object_ptr<Ui::PlainShadow> _topTabsBackground = { nullptr };
	//object_ptr<Ui::SettingsSlider> _topTabs = { nullptr };
	object_ptr<TopBar> _topBar = { nullptr };
	object_ptr<Ui::RpWidget> _topBarSurrogate = { nullptr };
	Animation _topBarOverrideAnimation;
	bool _topBarOverrideShown = false;
	object_ptr<Ui::FadeShadow> _topShadow;
	base::unique_qptr<Ui::IconButton> _topBarMenuToggle;
	base::unique_qptr<Ui::DropdownMenu> _topBarMenu;

//	Tab _tab = Tab::Profile;
//	std::unique_ptr<ContentMemento> _anotherTabMemento;
	std::vector<StackItem> _historyStack;

	rpl::event_stream<rpl::producer<int>> _desiredHeights;
	rpl::event_stream<rpl::producer<bool>> _desiredShadowVisibilities;
	rpl::event_stream<rpl::producer<SelectedItems>> _selectedLists;
	rpl::event_stream<rpl::producer<int>> _scrollTillBottomChanges;

};

} // namespace Info
