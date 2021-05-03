#ifndef Job_h
#define Job_h

class Job {
public:
  enum Type {
    SteerParallel,
    DriveParallel,
    SteerSpin,
    DriveSpin,    
    None
  };
  Job(Type type=Type::None, int param1 = 0, float param2 = 0) : type(type), param1(param1), param2(param2) {};

  int param1;           
  float param2;
  Type type;
};

#endif;
