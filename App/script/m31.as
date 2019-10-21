class BBM31 : BulletBehavior{
    float speed;
    float maxSpeed;
    float accel;
    bool flag = false;

    BBM31(float pSpeed, float pMaxSpeed, float pAccel){
        speed = pSpeed;
        maxSpeed = pMaxSpeed;
        accel = pAccel;
    }

    void Update(GameInterface &inout inter, BulletData &inout data){
        if(flag){
            data.pos += data.angle * speed;
            speed += accel;
            if(accel > 0) if(speed > maxSpeed)speed = maxSpeed;
            if(accel < 0) if(speed < maxSpeed)speed = maxSpeed;
        }
    }
    void Draw(const BulletData &inout data, const BulletContainer &inout c){}
}

class M31 : EnemyBehavior{
    Texture tex(GetResource("AS/m31.png"));
    BulletContainer c = GetBulletS(0.8);
    BulletContainer c2 = GetBulletS(1.6, ColorF(1.0, 1.0, 0.95));

    void Update(GameInterface &inout inter, EnemyData &inout data){
        repeat m = 70;
        BBM31 b(0.5, 2.5, 0.01);
        inter.aStardust = true;
        while(m()){
            repeat k = 2;
            while(k()){
                repeat i = (m.max - m) / 8;
                while(i()){
                    float r = shake();
                    inter.RegisterBullet(c, BulletData(data.pos + angle(m * dtr(10) + radial(k)) * (40 + m * 2) + r * Random(float(m.max) / 6) * angle(r), r), @b);          
                }
            }
            wait();
        }
        inter.aStardust = false;
        m = 15;
        while(m()){
            repeat i = 48;
            while(i()){
                inter.RegisterBullet(c2, BulletData(data.pos, shake(10) + radial(i)), BBStraightAccel(0, 6, 0.02));          
            }
            inter.aStar2 = true;
            wait(20);
            if(m == 3){
                b.flag = true;
                inter.aStar1 = true;
            }
        }
        wait(60);
    }

    void Draw(const EnemyData& data){
        tex.rotated(dtr(-45)).drawAt(data.pos);
    }
};