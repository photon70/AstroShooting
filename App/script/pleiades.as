class BBPleiades : BulletBehavior{
    float speed;
    float accel;
    float subSpeed;
    bool flag = false;

    BBPleiades(){}

    BBPleiades(float pSpeed, float pAccel, int pSubSpeed){
        speed = pSpeed;
        accel = pAccel;
        subSpeed = pSubSpeed + 1;
    }

    void Update(GameInterface &inout inter, BulletData &inout data){
        Vec2 angle = data.angle;
        angle = Mat3x2::Rotate(accel).transform(angle);
        data.pos += angle * speed;
        data.angle = angle;

        if(data.count % ((8 - subSpeed) * 20) == 10){
            repeat i = 12;
            while(i()){
                inter.RegisterBullet(GetBulletS(1.0, ColorF(0.8, 0.95, 1.0)), BulletData(data.pos, radial(i) + shake(10)), BBStraight(1 + float(subSpeed) * 0.3));
            }
        }

        if(flag){
            data.deleteFlag = true;
            repeat i = 48;
            while(i()){
                inter.RegisterBullet(GetBulletS(1, ColorF(0.8, 0.95, 1.0)), BulletData(Float2(0, -200), radial(i) + shake(20)), BBCurve(2+ float(subSpeed) * 0.3, shake(0.4)));
            }
        }
    }
    void Draw(const BulletData &inout data, const BulletContainer &inout c){}
};

class Pleiades : EnemyBehavior{
    Texture tex(GetResource("AS/star_1.png"));
    ColorF color = ColorF(0.7, 0.9, 1.0);
    BulletContainer c = GetBulletM(1.2, color);
    float radius = 150;
    Array<float> f = {2, 3, 5, 7, 11, 13, 17};

    void Update(GameInterface &inout inter, EnemyData &inout data){
        repeat m = 7;
        Array<BBPleiades> b;
        while(m()){
            float th = f[m] / 80;
            b << BBPleiades(th * radius, th, m);
    //  inter.RegisterBullet(c, BulletData(data.pos, radial(m)).AutoDelete(), @b[m]);
        }
        repeat k = 10000;
        while(k()){
            repeat m = 7;
            while(m()){
                if(k == int(Math::TwoPi / b[m].accel)){
            inter.aStar2 = true;
            inter.aStardust = true;
                    inter.RegisterBullet(c, BulletData(data.pos, radial(m)).AutoDelete(), @b[m]);
                }
            }
            if(k == int(Math::TwoPi / b[0].accel * f[0])){
                m = 7;
                while(m()){
                    b[m].flag = true;
                }
                inter.aStar1 = true;
                inter.aStardust = true;
                break;
            }
            wait();
        }
        wait(60);
    }

    void Draw(const EnemyData& data){
        tex.scaled(0.8).drawAt(data.pos + Point(-5, -32), color);
        tex.scaled(0.8).drawAt(data.pos + Point(-3, -20), color);
        tex.scaled(0.8).drawAt(data.pos + Point(19, -25), color);
        tex.scaled(0.8).drawAt(data.pos + Point(18, 0), color);
        tex.scaled(1).drawAt(data.pos + Point(0, 7), color);
        tex.scaled(0.8).drawAt(data.pos + Point(-10, 30), color);
        tex.scaled(0.5).drawAt(data.pos + Point(-16, 26), color);
    }
};