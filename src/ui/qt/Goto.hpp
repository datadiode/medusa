#ifndef __GOTO_H__
#define __GOTO_H__

#include <QDialog>
#include <QWidget>
#include "ui_Goto.h"
#include <medusa/address.hpp>

namespace Ui
{
  class Goto;
}

class Goto : public QDialog, public Ui::Goto
{
public:
  Goto(QWidget * parent = 0);
  ~Goto();

public:
  Ui::Goto *ui;
  medusa::Address address(void) const;
};

#endif // !__GOTO_H__