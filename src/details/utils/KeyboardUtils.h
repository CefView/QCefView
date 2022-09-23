#pragma once
#include <QKeyEvent>

#include <include/cef_app.h>

void
MapQKeyEventToCefKeyEvent(QKeyEvent* qe, CefKeyEvent& ce);

bool
ShouldSendKeyCharEvent(QKeyEvent* qe);

void
AdjustCefKeyCharEvent(QKeyEvent* qe, CefKeyEvent& ce);
