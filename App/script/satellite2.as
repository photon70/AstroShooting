class Satellite2 : EnemyBehavior{
    Texture tex(GetResource("AS/star_1.png"));
    BulletContainer c = GetBulletS(1.2, ColorF(0.6, 0.9, 1.0));

    void Update(GameInterface &inout inter, EnemyData &inout data){
        // repeat n = 30;
        // while(n()){
        //     data.pos = Bezier(Float2(0, -200), Float2(0, -300), n.progressO());
        //     wait();
        // }

        // while(true){
        //     repeat m = 3600;
        //     while(m()){
        //         if(m.skip(40)){
        //             float r = shake();
        //             repeat k = 20;
        //             while(k()){
        //                 inter.RegisterBullet(c_bs, BulletData(data.pos, r + radial(k)), BBStraight(3.2));
        //             }
        //         }

        //         if(m.skip(60)){
        //             repeat i = 3;
        //             while(i()){
        //                 repeat k = 16;
        //                 while(k()){
        //                     inter.RegisterBullet(c, BulletData(data.pos, aim(data.pos, inter.player) + nway(i, dtr(30)), 1.0 + k * 0.04), BBStraight(4.5));
        //                 }
        //             }
        //         }

        //         data.pos.x = 300 * Sin(2 * m.progress() * Math::TwoPi);
        //         data.pos.y = -300 * Cos(6 * m.progress() * Math::TwoPi);
        //         wait();
        //     }
        // }

        Array<Float2> p = {
                Float2(0, -200),
                Float2(150, -350),
                Float2(300, -200),
                Float2(150, -50),
                Float2(300, -200),
                Float2(150, -350),
                Float2(0, -200),
                Float2(-150, -300),
                Float2(-300, -200),
                Float2(-150, -50),
                Float2(-300, -200),
                Float2(-150, -350),
                Float2(0, -200),
            };
        repeat n = p.size() - 1;
        while(n()){
            repeat m = 10;
            Float2 vec = p[n+1] - p[n];
            if(n > 0)
                vec -= (p[n] - p[n-1]);
            float rad = atan(vec) + dtr(180);
            while(m()){
                repeat k = 3;
                while(k()){
                    inter.RegisterBullet(c_bs, BulletData(data.pos, rad + shake(5) + nway(m, dtr(30))/ 2, 1.0 - k * 0.2), BBStraight(3));
                }
            }

            m = 10;
            while(m()){
                data.pos = Bezier(p[n], p[n + 1], EaseOutSine(m.progressO()));
                inter.RegisterBullet(c_bs, BulletData(data.pos, atan(p[n+1] - p[n]) + shake(4)), BBStraight(2.5));
                if(m.skip(10)){
                    float rad = aim(data.pos, inter.player);
                    repeat i = 10;
                    while(i()){
                        inter.RegisterBullet(c, BulletData(data.pos + (80 - i * 4) * angle(rad), rad, 1.0 - i * 0.04), BBStraight(5.5));
                    }
                }
                wait();
            }
        }
        wait(90);
            repeat m = 10;
            while(m()){
                float rad = aim(data.pos, inter.player) + dtr(20) * wave(m, m.max) + shake(3);
                repeat i = 10;
                while(i()){
                    inter.RegisterBullet(c, BulletData(data.pos + 20 * angle(rad), rad, 1.0 - i * 0.04), BBStraight(5.5));
                }
                wait(4);
            }
        wait(120);
    }

    void Draw(const EnemyData& data){
        tex.drawAt(data.pos);
    }
};