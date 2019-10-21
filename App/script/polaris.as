// class Polaris : EnemyBehavior{
//     Texture tex(GetResource("AS/star_1.png"));
//     BulletContainer c = GetBulletS();
//     BulletContainer c2 = GetBulletS(1.2, ColorF(1.0, 1.0, 0.9));

//     void Update(GameInterface &inout inter, EnemyData &inout data){
//         inter.aStardust = true;
//         repeat m = 1440;
//         while(m()){
//         //    data.pos = Mat3x2::Rotate(-dtr(m) / 4).transform(Point(0, -200));
//             repeat k = 4;
//             while(k()){
//                 float rad = dtr(m) * 2 + radial(k);
//                 inter.RegisterBullet(c, BulletData(data.pos + angle(rad) * 20, rad), @BBStraight(4));
//             }
//             k = 16;
//             if(m.skip(8)){
//                 while(k()){
//                     float rad = -dtr(m) * 4 + radial(k) + shake(10);
//                     inter.RegisterBullet(c2, BulletData(data.pos + angle(rad) * 20, rad), @BBStraight(2));
//                 }
//             inter.aStar2 = true;
//             }
//             wait(4);
//         }
//     }

//     void Draw(const EnemyData& data, const BulletContainer &inout c){
//         tex.drawAt(data.pos, ColorF(1.0, 1.0, 0.7));
//     }
// };

class BBPolaris : BulletBehavior{
    float speed;
    float accel;
    Float2 pos;

    BBPolaris(float pSpeed, float pAccel, Float2 pPos){
        speed = pSpeed;
        accel = pAccel;
        pos = pPos;
    }

    void Update(GameInterface &inout inter, BulletData &inout data){
        data.angle = Mat3x2::Rotate(accel).transform(data.angle);
        data.pos += data.angle * speed;
        if(data.count > 30 && data.count % 2 == 0)
            inter.RegisterBullet(c_bs, BulletData(data.pos, aim(Float2(0, 0), data.angle) + Math::Pi + shake(5)), BBStraight(2.5));
        if(float(data.count) > Math::TwoPi / Abs(accel))
            data.deleteFlag = true;
    }
    void Draw(const BulletData &inout data, const BulletContainer &inout c){}
};

class Polaris : EnemyBehavior{
    Texture tex(GetResource("AS/star_1.png"));
    BulletContainer c = GetBulletS();
    BulletContainer c2 = GetBulletS(1.2, ColorF(1.0, 1.0, 0.9));
    void Update(GameInterface &inout inter, EnemyData &inout data){
        //inter.aStardust = true;
        // repeat m = 1440;
        // while(m()){
        // //    data.pos = Mat3x2::Rotate(-dtr(m) / 4).transform(Point(0, -200));
        //     repeat k = 4;
        //     while(k()){
        //         float rad = dtr(m) * 2 + radial(k);
        //         inter.RegisterBullet(c, BulletData(data.pos + angle(rad) * 20, rad), @BBStraight(4));
        //     }
        //     k = 16;
        //     if(m.skip(8)){
        //         while(k()){
        //             float rad = -dtr(m) * 4 + radial(k) + shake(10);
        //             inter.RegisterBullet(c2, BulletData(data.pos + angle(rad) * 20, rad), @BBStraight(2));
        //         }
        //     inter.aStar2 = true;
        //     }
        //     wait(4);
        // }
        inter.aStar1 = true;
        repeat k = 7;
        float r = shake();
        while(k()){
            inter.RegisterBullet(c2, BulletData(data.pos, r + radial(k)).AutoDelete(), BBPolaris(3, -0.01, data.pos));
        } 
        wait(300);
    }
    void Draw(const EnemyData& data){
        tex.drawAt(data.pos, ColorF(1.0, 1.0, 0.7));
    }
};