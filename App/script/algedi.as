class Algedi : EnemyBehavior{
    Texture tex("AS/star_1.png");
    BulletContainer c = GetBulletS(1.0, ColorF(1.0, 1.0, 0.8));
    BulletContainer c2 = GetBulletS(1.5, ColorF(1.0, 1.0, 0.7));
    BBStraightA b(6, 80, 6), b2(1.6, 120, 10);

    void Update(GameInterface &inout inter, EnemyData &inout data){
        repeat i = 6;
        float rad = aim(data.pos, inter.player);
        while(i()){
            repeat j = 9;
            while(j())
                inter.RegisterBullet(c2, BulletData(data.pos + Point(40, 10), nway(j, rad, dtr(12)) + shake(4), 1.0, rand(15)), @b2);
            wait(30);
        }
        wait(30);
        
        rad = aim(data.pos, inter.player);
        i = 10;
        while(i()){
            repeat k = 30;
            while(k()){
                inter.RegisterBullet(c, BulletData(data.pos, radialOut(k, rad) + dtr(1 * i)) ,@b);
            }
            wait(6);
        }
        i = 10;
        while(i()){
            repeat k = 30;
            while(k()){
                inter.RegisterBullet(c, BulletData(data.pos, radialOut(k, rad) + dtr(-1 * i)) ,@b);
            }
            wait(6);
        }
        i = 10;
        while(i()){
            repeat k = 30;
            while(k()){
                inter.RegisterBullet(c, BulletData(data.pos, radialOut(k, rad + dtr(3)) + dtr(1 * i)) ,@b);
            }
            wait(3);
        }
    }

    void Draw(const EnemyData& data){
        tex.scaled(1.2).drawAt(data.pos, ColorF(1.0, 1.0, 0.6));
        tex.scaled(0.8).drawAt(data.pos + Point(40, 10), ColorF(1.0, 1.0, 0.6));
    }
};