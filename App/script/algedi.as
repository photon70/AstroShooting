class Algedi : EnemyBehavior{
    Texture tex(GetResource("AS/star_1.png"));
    BulletContainer c = GetBulletS(1.0, ColorF(1.0, 1.0, 0.8));
    BulletContainer c2 = GetBulletS(1, ColorF(1.0, 1.0, 0.7));
    BBStraightA b(4, 80, 6), b2(1.2, 120, 10);

    void Update(GameInterface &inout inter, EnemyData &inout data){
        repeat m = 2;
        while(m()){
            repeat i = 8;
            float rad = aim(data.pos, inter.player);
            while(i()){
                repeat j = 9;
                while(j())
                    inter.RegisterBullet(c2, BulletData(data.pos + Point(40, 10), nway(j, rad, dtr(12)) + shake(4), 1.0, rand(15)), @b2);
                inter.aStar2 = true;
                wait(30);
            }
            rad = aim(data.pos, inter.player);
            inter.aStardust = true;
            i = 10;
            while(i()){
                repeat k = 30;
                while(k()){
                    inter.RegisterBullet(c, BulletData(data.pos, sign(m) * radialOut(k, rad) + sign(m) *dtr(1 * i)) ,@b);
                }
                wait(6);
            }
            i = 10;
            while(i()){
                repeat k = 30;
                while(k()){
                    inter.RegisterBullet(c, BulletData(data.pos, sign(m) * radialOut(k, rad) + sign(m) *dtr(-1 * i)) ,@b);
                }
                wait(6);
            }
            i = 10;
            while(i()){
                repeat k = 30;
                while(k()){
                    inter.RegisterBullet(c, BulletData(data.pos, sign(m) * radialOut(k, rad + dtr(3)) + sign(m) *dtr(1 * i)) ,@b);
                }
                wait(3);
            }
            inter.aStardust = false;
        }
    }

    void Draw(const EnemyData& data){
        tex.scaled(1.2).drawAt(data.pos, ColorF(1.0, 1.0, 0.6));
        tex.scaled(0.8).drawAt(data.pos + Point(40, 10), ColorF(1.0, 1.0, 0.6));
    }
};