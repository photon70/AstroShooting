//基本の弾の定義
BulletContainer GetBulletM(float scale = 1.0, ColorF color = Palette::White){
    BulletContainer b;
    b.texture = Texture(GetResource("AS/star_1.png"));
    b.color = color;
    b.scale = scale;
    b.rad = 4.0 * scale;
    b.score = 100;
    return b;
}

BulletContainer GetBulletS(float scale = 1.0, ColorF color = Palette::White){
    BulletContainer b;
    b.texture = Texture(GetResource("AS/star_2.png"));
    b.color = color;
    b.scale = scale;
    b.rad = 3.0 * scale;
    b.score = 100;
    return b;
}

BulletContainer c_bs = GetBulletS(1);

//基本関数
class repeat{
    int count = -1;
    int max = 0;

    repeat(){}

    repeat(int n){
        count = -1;
        max = n;
    }

    float center(){
        return float(count) - float(max - 1) / 2;
    }

    bool skip(int time){
        return (count % time == 0);
    }

    bool alternate(int time){
        return (count / time % 2 == 0);
    }

    double progress(){
        return double(count) / double(max);
    }

    double progressO(){
        return double(count) / double(max - 1);
    }

    void opAssign(int n){
        count = -1;
        max = n;
    }

    bool opCall() {
        ++count;
        return count < max; 
    }

    int opImplConv()const {
        return count;
    }

    int opNeg()const {
        return -count;
    }
}

class Locate{
    Float2 vec;

    Locate(){
        
    }

    Locate(const Float2 &in shooter, const Float2 &in target){
        vec = target - shooter;
    }

    Locate(Float2 target){
        vec = target;
    }

    void Avairable(const Float2 &in shooter){
        vec -= shooter;
    }

    Float2 opCall(float t, float max) {
        return vec / max; 
    }
}

void wait(int n = 1){
    for(int i = 0; i < n; ++i)
        yield();
}

float dtr(float deg){
    return deg / 360 * Math::TwoPi;
}

Float2 angle(float radian){
    radian -= dtr(90);
    return Float2(Math::Cos(radian), Math::Sin(radian));
}

float shake(float range = 180){
    range = dtr(range);
    return Random(-range, range);
}

Float2 randCir(float radius){
    radius = Random(radius);
    float radian = shake();
    return radius * Float2(Math::Cos(radian), Math::Sin(radian));
}

float sign(int n){
    if(n % 2 == 0){
        return 1;
    }
    else{
        return -1;
    }
}

int rand(int max){
    return Random(max);
}

int rand(int min, int max){
    return Random(min, max);
}


float linear(int t, int period, float ampl = 1.0) {
	return ampl * t / period;
}

float wave(int t, int period, float ampl = 1.0) {
	return ampl * Sin(Math::TwoPi * t / period);
}

float waves(int t, int period, float ampl = 1.0) {
	return ampl * Sin(Math::TwoPi * t / period);
}

float wavec(int t, int period, float ampl = 1.0) {
	return ampl * Cos(Math::TwoPi * t / period);
}

float nway(int num, float center, float rad, int i) {
	return (center - (num - 1) * rad / 2 + rad * i);
}

float nway(repeat r, float center, float rad) {
	return (center - (r.max - 1) * rad / 2 + rad * r);
}

float nway(repeat r, float rad) {
	return (- (r.max - 1) * rad / 2 + rad * r);
}

float radial(int num, float center, int i) {
	return (center + 2 * Math::Pi * i / num);
}

float radial(repeat r, float center = 0) {
	return (center + 2 * Math::Pi * r.progress());
}

float radialOut(int num, float center, int i) {
	return (center + Math::TwoPi * i / num + Math::Pi / num);
}

float radialOut(repeat r, float center = 0) {
	return (center + Math::TwoPi * r.progress() + Math::Pi / r.max);
}

float aim(Float2 shooter, Float2 target){
    return Math::Atan2(target.y - shooter.y, target.x - shooter.x) + Math::HalfPi;
}

float atan(Float2 vec){
    return Math::Atan2(vec.y, vec.x) + Math::HalfPi;
}

float Bezier(float r1, float r2, double t){
    return (1.0 - t) * r1 + t * r2;
}

Float2 Bezier(Float2 pos1, Float2 pos2, double t){
    return (1.0 - t) * pos1 + t * pos2;
}

Float2 Bezier(Float2 pos1, Float2 pos2, Float2 pos3, double t){
    return (1.0 - t) * Bezier(pos1, pos2, t) + t * Bezier(pos2, pos3, t);
}

bool arrange(BulletData &inout data, float radius, float time){
    if(data.count < time){
        data.pos += data.angle * radius / time;
        return false;
    }
    else{
        return true;
    }
}

bool arrange(BulletData &inout data, Float2 vec, float time){
    if(data.count < time){
        data.pos += vec / time;
        return false;
    }
    else{
        return true;
    }
}

bool arrange(BulletData &inout data, Locate &inout locate, float time){
    if(data.count < time){
        data.pos += locate(data.count, time);
        return false;
    }
    else{
        return true;
    }
}

//基本のBehaviorの定義

class BBStraight : BulletBehavior{
    float speed;

    BBStraight(float pSpeed){
        speed = pSpeed;
    }

    void Update(GameInterface &inout inter, BulletData &inout data){
        data.pos += data.angle * speed;
    }

    void Draw(const BulletData &inout data, const BulletContainer &inout c){}
};

class BBStraightA : BulletBehavior{
    float speed;
    float radius;
    float time;

    BBStraightA(float pSpeed, float pRadius, float pTime){
        speed = pSpeed;
        radius = pRadius;
        time = pTime;
    }

    void Update(GameInterface &inout inter, BulletData &inout data){
        if(arrange(data, radius, time))
            data.pos += data.angle * speed;
    }
    void Draw(const BulletData &inout data, const BulletContainer &inout c){}
};

class BBStraightAccel : BulletBehavior{
    float speed;
    float maxSpeed;
    float accel;

    BBStraightAccel(float pSpeed, float pMaxSpeed, float pAccel){
        speed = pSpeed;
        maxSpeed = pMaxSpeed;
        accel = pAccel;
    }

    void Update(GameInterface &inout inter, BulletData &inout data){
        data.pos += data.angle * speed;
        speed += accel;
        if(accel > 0) if(speed > maxSpeed)speed = maxSpeed;
        if(accel < 0) if(speed < maxSpeed)speed = maxSpeed;
    }
    void Draw(const BulletData &inout data, const BulletContainer &inout c){}
};

class BBStraightAccelA : BulletBehavior{
    float speed;
    float maxSpeed;
    float accel;
    float radius;
    float time;

    BBStraightAccelA(float pSpeed, float pMaxSpeed, float pAccel, float pRadius, float pTime){
        speed = pSpeed;
        maxSpeed = pMaxSpeed;
        accel = pAccel;
        radius = pRadius;
        time = pTime;
    }

    void Update(GameInterface &inout inter, BulletData &inout data){
        if(arrange(data, radius, time)){
            data.pos += data.angle * speed;
            speed += accel;
            if(accel > 0) if(speed > maxSpeed)speed = maxSpeed;
            if(accel < 0) if(speed < maxSpeed)speed = maxSpeed;
        }
    }
    void Draw(const BulletData &inout data, const BulletContainer &inout c){}
};

class BBCurve : BulletBehavior{
    float speed;
    float accel;

    BBCurve(float pSpeed, float pAccel){
        speed = pSpeed;
        accel = pAccel;
    }

    void Update(GameInterface &inout inter, BulletData &inout data){
        data.angle += Mat3x2::Rotate(dtr(90)).transform(data.angle).normalized() * accel;
        data.pos += data.angle * speed;
    }
    void Draw(const BulletData &inout data, const BulletContainer &inout c){}
};

class BBCurveS : BulletBehavior{
    float speed;
    float accel;

    BBCurveS(float pSpeed, float pAccel){
        speed = pSpeed;
        accel = pAccel;
    }

    void Update(GameInterface &inout inter, BulletData &inout data){
        data.angle = Mat3x2::Rotate(accel).transform(data.angle);
        data.pos += data.angle * speed;
    }
    void Draw(const BulletData &inout data, const BulletContainer &inout c){}
};

class BBCurveA : BulletBehavior{
    float speed;
    float accel;
    float radius;
    float time;

    BBCurveA(float pSpeed, float pAccel, float pRadius, float pTime){
        speed = pSpeed;
        accel = pAccel;
        radius = pRadius;
        time = pTime;
    }

    void Update(GameInterface &inout inter, BulletData &inout data){
        if(arrange(data, radius, time)){
            data.angle += Mat3x2::Rotate(dtr(90)).transform(data.angle).normalized() * accel;
            data.pos += data.angle * speed;
        }
    }
    void Draw(const BulletData &inout data, const BulletContainer &inout c){}
};

class BBCurveSAW : BulletBehavior{
    float speed;
    float accel;
    float radius;
    float time;
    float wait;

    BBCurveSAW(float pSpeed, float pAccel, float pRadius, float pTime, float pWait){
        speed = pSpeed;
        accel = pAccel;
        radius = pRadius;
        time = pTime;
        wait = pWait;
    }

    void Update(GameInterface &inout inter, BulletData &inout data){
        if(arrange(data, radius, time) && data.count > time + wait){
            data.angle = Mat3x2::Rotate(accel).transform(data.angle);
            data.pos += data.angle * speed;
        }
    }
    void Draw(const BulletData &inout data, const BulletContainer &inout c){}
};