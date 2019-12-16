#ifndef BODY_H_
#define BODY_H_

#include <memory>
#include <string>
#include <vector>

class Body
{
public:
  typedef std::shared_ptr<Body> Ptr;

  Body(std::string name, Body::Ptr parent)
  {
    name_ = name;
    parent_body_ = parent;
  }

  Body::Ptr getParentBody()
  {
    return parent_body_;
  }

  void setParentBody(Body::Ptr parent_body)
  {
    parent_body_ = parent_body;
  }

  std::vector<Body::Ptr> getChildrenBodies()
  {
    return orbiting_children_;
  }

  void addOrbitingChild(Body::Ptr body)
  {
    orbiting_children_.push_back(body);
  }

private:
  std::string name_;
  Body::Ptr parent_body_;
  std::vector<Body::Ptr> orbiting_children_;
};

#endif  // BODY_H_