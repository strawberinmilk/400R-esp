#ifndef BASE_SETTING_H
#define BASE_SETTING_H

class BaseSetting
{
public:
  const char *name;

  // 仮想デストラクタ
  virtual ~BaseSetting() = default;

  // 純粋仮想関数として宣言
  virtual void start() = 0;
  virtual void update() = 0;
  virtual void cleanup() = 0;
};

#endif
