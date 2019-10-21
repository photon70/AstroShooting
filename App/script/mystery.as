class Mystery : EnemyBehavior{
    Texture tex(GetResource("AS/star_1.png"));
    Array<Vec2> p = {Vec2(-300, -200), Vec2(300, -200), Vec2(0, -200), Point(0, 0)};

    void Update(GameInterface &inout inter, EnemyData &inout data){
        repeat i = 3;
        while(i()){
            float rad = aim(data.pos, inter.player);
            repeat n = 3;
            while(n()){
                int num = 12;
                repeat k = 2;
                while(k()){
                    repeat m = num - k;
                    while(m()){
                        float r = shake(3);
                        repeat i = 4;
                        while(i()){
                            inter.RegisterBullet(c_bs, BulletData(data.pos, rad + r + nway(m, dtr(10)) + nway(i, dtr(1))), BBStraight(2.5));
                        }
                    }
                    wait(30);
                }
            }
            n = 20;
            while(n()){
                data.pos = Bezier(p[(i + 2) % 3], p[i], n.progress());
                wait();
            }
        }
    }

    void Draw(const EnemyData& data){
        tex.scaled(0.5).drawAt(data.pos);
    }
};