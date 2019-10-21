class BBGarnetStar : BulletBehavior{
    float speed;
    float accel;

    BBGarnetStar(float pSpeed, float pAccel){
        speed = pSpeed;
        accel = pAccel;
    }

    void Update(GameInterface &inout inter, BulletData &inout data){
        data.pos += data.angle * speed;
        data.angle = Mat3x2::Rotate(accel).transform(data.angle);
        if(Math::TwoPi / Abs(accel) < data.count)
            data.deleteFlag = true;
    }
    void Draw(const BulletData &inout data, const BulletContainer &inout c){}
};

class GarnetStar : EnemyBehavior{
    Texture tex(GetResource("AS/star_1.png"));
    BulletContainer c = GetBulletS(1, ColorF(1.0, 0.6, 0.4));

    void Update(GameInterface &inout inter, EnemyData &inout data){
        inter.aStar1 = true;
        inter.aStardust = true;
        float rotate = shake();
        repeat k = 10;
        while(k()){
            BBGarnetStar b1(2, dtr(0.25 + 0.03 * k));
            BBGarnetStar b2(2, -dtr(0.3 + 0.03 * k));
            repeat m = 24;
            while(m()){
                repeat n = 3;
                float r = shake(2);
                while(n()){
                    inter.RegisterBullet(c, BulletData(data.pos, rotate + r + radial(m) - dtr(k), 1.0, n * 8).AutoDelete(), @b1);
                    inter.RegisterBullet(c, BulletData(data.pos, rotate + r + radial(m) + dtr(k), 1.0, n * 8).AutoDelete(), @b2);
                }
            }
            wait(24);
        }
        inter.aStar2 = true;
        inter.aStardust = false;
        wait(700);
    }

    void Draw(const EnemyData& data){
        tex.drawAt(data.pos, ColorF(1.0, 0.6, 0.4));
    }
};