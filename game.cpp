#include<iostream>
#include<math.h>
#include<algorithm>
#include<vector>
#include<set>
#include<cstdlib>
#include<ctime>
#include<fstream>
#include<SDL3/SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include<SDL_mixer.h>

using namespace std;


// 1. BIẾN TOÀN CỤC (GLOBAL VARIABLES)
SDL_Window *Window = nullptr;
SDL_Renderer *Renderer = nullptr;
SDL_Event Event;
TTF_Font *Font = nullptr;
MIX_Mixer *Mix;
bool running = true;


// 2. LỚP CƠ SỞ CHO CÁC LOẠI QUẢ (BASE CLASS FOR FRUIT TYPES)
class Qua {
protected:
    SDL_Texture* qua;
    int xqua;
    int yqua;
    int loaiqua;
    long long diem;
    SDL_Renderer* Renderer;
    
public:
    // CONSTRUCTOR
    Qua(SDL_Renderer* Renderer) {
        this->Renderer = Renderer;
    }
    
    // DESTRUCTOR (VIRTUAL)
    virtual ~Qua() {
        if (qua) SDL_DestroyTexture(qua);
    }
    
    // PHƯƠNG THỨC THUẦN ẢO (PURE VIRTUAL METHOD)
    virtual void taoqua(float xqua, float yqua) = 0;
    
    // HÀM GETTER
    int getXqua() { return xqua; }
    int getYqua() { return yqua; }
    int getLoaiqua() { return loaiqua; }
    int getdiem() { return diem; }
    
    // HÀM SETTER
    void setXqua(int x) { this->xqua = x; }
    void setYqua(int y) { this->yqua = y; }
    void setLoaiqua(int loaiqua) { this->loaiqua = loaiqua; }
};


// 3. CÁC LỚP QUẢ KẾ THỪA (DERIVED FRUIT CLASSES)
class QuaTao : public Qua {
public:
    // CONSTRUCTOR
    QuaTao(SDL_Renderer* Renderer) : Qua(Renderer) {
        qua = IMG_LoadTexture(Renderer, "pictures/tao2-removebg-preview.png");
        loaiqua = 1;
        diem = 10;
    }
    
    // PHƯƠNG THỨC VẼ QUẢ
    void taoqua(float xqua, float yqua) override {
        SDL_FRect vitriqua = {xqua - 5, yqua - 5, 50, 50};
        SDL_RenderTexture(Renderer, qua, nullptr, &vitriqua);
    }
};

class QuaThit : public Qua {
public:
    // CONSTRUCTOR
    QuaThit(SDL_Renderer* Renderer) : Qua(Renderer) {
        qua = IMG_LoadTexture(Renderer, "pictures/thit-removebg-preview.png");
        loaiqua = 2;
        diem = 15;
    }
    
    // PHƯƠNG THỨC VẼ QUẢ
    void taoqua(float xqua, float yqua) override {
        SDL_FRect vitriqua = {xqua - 5, yqua - 5, 50, 50};
        SDL_RenderTexture(Renderer, qua, nullptr, &vitriqua);
    }
};

class QuaCam : public Qua {
public:
    // CONSTRUCTOR
    QuaCam(SDL_Renderer* Renderer) : Qua(Renderer) {
        qua = IMG_LoadTexture(Renderer, "pictures/cam2-removebg-preview.png");
        loaiqua = 3;
        diem = 20;
    }
    
    // PHƯƠNG THỨC VẼ QUẢ
    void taoqua(float xqua, float yqua) override {
        SDL_FRect vitriqua = {xqua - 5, yqua - 5, 50, 50};
        SDL_RenderTexture(Renderer, qua, nullptr, &vitriqua);
    }
};


// 4. LỚP RẮN (SNAKE CLASS)
class Ran{
private:
    // THUỘC TÍNH ẢNH
    SDL_Texture *background;
    SDL_Texture *thanran;
    SDL_Texture *dauranphai;
    SDL_Texture *daurantren;
    SDL_Texture *dauranduoi;
    SDL_Texture *daurantrai;
    
    // THUỘC TÍNH LOGIC
    int huong;
    float thoigian;
    float mucthoigian;
    float buoc;
    int dodairanbandau;
    int dodairan;
    int nhanphim;
    bool quabian;
    int loaiqua;
    float xcuoi, ycuoi;
    vector<float> toadox;
    vector<float> toadoy;
    vector<float> toadox_truoc;
    vector<float> toadoy_truoc;
    
public:
    // CONSTRUCTOR
    Ran(SDL_Renderer *Renderer){
        quabian = true;
        xcuoi = -100;
        ycuoi = -100;
        background = IMG_LoadTexture(Renderer, "pictures/bgmenu1.png");
        dauranphai = IMG_LoadTexture(Renderer, "pictures/dauranphai-removebg-preview.png");
        daurantrai = IMG_LoadTexture(Renderer, "pictures/daurantrai-removebg-preview.png");
        daurantren = IMG_LoadTexture(Renderer, "pictures/daurantren-removebg-preview.png");
        dauranduoi = IMG_LoadTexture(Renderer, "pictures/dauranduoi-removebg-preview.png");
        thanran = IMG_LoadTexture(Renderer, "pictures/thanran-removebg-preview.png");
    }
    
    // DESTRUCTOR
    ~Ran() {
        SDL_DestroyTexture(background);
        SDL_DestroyTexture(thanran);
        SDL_DestroyTexture(dauranphai);
        SDL_DestroyTexture(daurantren);
        SDL_DestroyTexture(dauranduoi);
        SDL_DestroyTexture(daurantrai);
    }

    // HÀM GETTER
    vector<float> &getToadox(){return toadox;}
    vector<float> &getToadoy(){return toadoy;}
    vector<float> &getToadox_truoc(){return toadox_truoc;}
    vector<float> &getToadoy_truoc(){return toadoy_truoc;}
    int getHuong(){return huong;}
    float getThoigian(){return thoigian;}
    float getMucthoigian(){return mucthoigian;}
    float getBuoc(){return buoc;}
    int getDodairanbandau(){return dodairanbandau;}
    int getDodairan(){return dodairan;}
    int getNhanphim(){return nhanphim;}
    bool getQuabian(){return quabian;}
    
    // HÀM SETTER
    void setHuong(int huong){this->huong = huong;}
    void setThoigian(float thoigian){this->thoigian = thoigian;}
    void setMucthoigian(float mucthoigian){this->mucthoigian = mucthoigian;}
    void setBuoc(float buoc){this->buoc = buoc;}
    void setDodairanbandau(int dodairanbandau){this->dodairanbandau = dodairanbandau;}
    void setDodairan(int dodairan){this->dodairan = dodairan;}
    void setNhanphim(int nhanphim){this->nhanphim = nhanphim;}
    void setQuabian(bool quabian){this->quabian = quabian;}
    
    // XỬ LÝ CHIỀU DÀI RẮN (DỊCH CHUYỂN CÁC ĐỐT RẮN)
    void xulychieudai(vector<float> &toado,int dodai, float &x, float &cuoi){
        cuoi = toado[dodai - 1];
        for (int i = dodai - 1; i > 0; i--) {
            toado[i] = toado[i - 1];
        }
        toado[0] = x;
    }
    
    // XỬ LÝ RẮN DI CHUYỂN THEO THỜI GIAN
    void logicgame(vector<float> &toadox, vector<float> &toadoy, float &x, float &y){
        thoigian += 0.1;
        if (huong == 1 && thoigian >= mucthoigian){ // PHẢI
            x += buoc;
            xulychieudai(toadox, dodairan, x, xcuoi);
            xulychieudai(toadoy, dodairan, toadoy[0], ycuoi);
            nhanphim = 1;
            thoigian = 0;
            
        }else if (huong == 2 && thoigian >= mucthoigian){ // XUỐNG
            y += buoc;
            xulychieudai(toadox, dodairan, toadox[0], xcuoi);
            xulychieudai(toadoy, dodairan, y, ycuoi);
            nhanphim = 1;
            thoigian = 0;
            
        }else if (huong == 3 && thoigian >= mucthoigian){ // TRÁI
            x -= buoc;
            xulychieudai(toadox, dodairan, x, xcuoi);
            xulychieudai(toadoy, dodairan, toadoy[0], ycuoi);
            nhanphim = 1;
            thoigian = 0;
            
        }else if (huong == 4 && thoigian >= mucthoigian){ // LÊN
            y -= buoc;
            xulychieudai(toadox, dodairan, toadox[0], xcuoi);
            xulychieudai(toadoy, dodairan, y, ycuoi); 
            nhanphim = 1;
            thoigian = 0;
        }
    }
    
    // KIỂM TRA ĐÂM TƯỜNG 
    bool ktrakhongdamtuong(vector<float> &toadox,vector<float> &toadoy){
        if (toadox[0] >= 80 && toadox[0] <= 1240 && toadoy[0] >= 40 && toadoy[0] <= 840)return true;
        return false;
    }
    
    // KIỂM TRA CẮN ĐUÔI 
    int ktrakhongcanduoi(vector<float> &toadox, vector<float> &toadoy, float x_next, float y_next){
        for (int i = 1; i < dodairan; i++){
            if (abs(x_next - toadox[i]) < 1 && abs(y_next - toadoy[i]) < 1){
                return 0;
            }
        }
        return 1;
    }
    
    // KIỂM TRA ĂN QUẢ VÀ TĂNG CHIỀU DÀI
    void ktraquabian(vector<float> &toadox, vector<float> &toadoy, float xqua, float yqua){
        if (abs(toadox[0] - xqua) < 6 && abs(toadoy[0] - yqua) < 6){
            dodairan++;
            toadox.push_back(-100); 
            toadoy.push_back(-100);
            quabian = true;
        }else{
            quabian = false;
        }
    }
    
    // KIỂM TRA KHÔNG CHO QUẢ TRÙNG THÂN RẮN
    int ktra_qua (float &xqua, float &yqua, vector<float> &toadox, vector<float> &toadoy){
        int dodai = toadox.size();
        for (int i = 0; i < dodai; i++){
            if(abs(xqua - toadox[i]) <= 0.1 && abs(yqua - toadoy[i]) <= 0.1)return 0;
        }
        return 1;
    }
    
    // LƯU TỌA ĐỘ TRƯỚC KHI CHẾT
    void luutruoc(const vector<float>& toadox, const vector<float>& toadoy){
        toadox_truoc = toadox;
        toadoy_truoc = toadoy;
    }
    
    // VẼ RẮN
    void veran(vector<float> &toadox, vector<float> &toadoy, int dodai){
        
        for (int i = 0; i < dodai; i++){
            if (i == 0){
                SDL_FRect vitridauphai = {toadox[i], toadoy[i], 80, 40};
                SDL_FRect vitridauduoi = {toadox[i], toadoy[i], 40, 80};
                SDL_FRect vitridautrai = {toadox[i] - 40, toadoy[i], 80, 40};
                SDL_FRect vitridautren = {toadox[i], toadoy[i] - 40, 40, 80};
                if(huong == 1){
                    SDL_RenderTexture(Renderer, dauranphai, nullptr, &vitridauphai);
                }else if (huong == 2){
                    SDL_RenderTexture(Renderer, dauranduoi, nullptr, &vitridauduoi);
                }else if (huong == 3){
                    SDL_RenderTexture(Renderer, daurantrai, nullptr, &vitridautrai);
                }else if (huong == 4){
                    SDL_RenderTexture(Renderer, daurantren, nullptr, &vitridautren);
                }
            }else{
                SDL_FRect vitrithan = {toadox[i], toadoy[i], 40, 40};
                SDL_RenderTexture(Renderer, thanran, nullptr, &vitrithan);
            }
        } 
    }

    void sukienran(SDL_Event& event) {
        if (( event.key.key == 'd' || event.key.key == SDLK_RIGHT ) && huong != 3 && nhanphim == 1){
            huong = 1;
            nhanphim = 0;
        }else if ((event.key.key == 's' || event.key.key == SDLK_DOWN) && huong != 4 && nhanphim == 1){
            huong = 2;
            nhanphim = 0;
        }else if ((event.key.key == 'a' || event.key.key == SDLK_LEFT) && huong != 1 && nhanphim == 1){
            huong = 3;
            nhanphim = 0;
        }else if ((event.key.key == 'w' || event.key.key == SDLK_UP) && huong != 2 && nhanphim == 1){
            huong = 4;
            nhanphim = 0;
        }
    }
    
};


// 5. LỚP BẢNG XẾP HẠNG
class Bangxephang{
private:
    fstream fs;
    int xephang[6]; // Lưu 5 hạng đầu và 1 slot cho điểm mới
    int diemhientai;
    
public:
    int xephangphu[6]; // Mảng public để menu truy cập
    
    // CONSTRUCTOR
    Bangxephang(){
        for (int i = 0; i < 6; i++){
            xephang[i] = 0;
        }
    }
    
    // DESTRUCTOR
    ~Bangxephang(){}
    
    // CẬP NHẬT MẢNG PUBLIC ĐỂ TRUY CẬP ĐƯỢC TỪ MENU
    void capnhatbangphu(){
        for (int i = 0; i < 6; i++){
            xephangphu[i] = xephang[i];
        }
    }
    
    // LẤY DỮ LIỆU TỪ FILE
    void docfile(){
        try {
            fs.open("bangxephang.txt", ios::in);
            if (!fs.is_open()) {
                throw "Khong the mo file 'bangxephang.txt' de doc."; 
            }
            for (int i = 0; i < 6; i++){
                if (!(fs >> xephang[i])) { 
                    throw "Du lieu file 'bangxephang.txt' bi loi dinh dang."; 
                }
            }
            fs.close();
        }
        catch (const char* msg) {
            cerr << "LOI XU LY BANG XEP HANG: " << msg << endl;
        }
    }
    
    // ĐƯA DỮ LIỆU VÀO FILE
    void ghifile(){
        fs.open("bangxephang.txt", ios::out);
        for (int i = 0; i < 6; i++){
            fs << xephang[i] << endl;
        }
        fs.close();
    }
    
    // SẮP XẾP LẠI BẢNG XẾP HẠNG
    void sapsep(){
        set<int, greater<int>> s(xephang, xephang + 6);
        int i = 0;
        for (int val : s) xephang[i++] = val;
        while (i < 6) xephang[i++] = 0;
    }
    
    // CẬP NHẬT ĐIỂM SAU KHI XONG MỘT MÀN (Đưa điểm mới vào slot cuối)
    void capnhatdiem(int diem){
        xephang[5] = diem;
    }
};



// 6. LỚP MÀN HÌNH CHÍNH/QUẢN LÝ GAME (MAIN GAME MANAGER CLASS)
class menu{
private:
    // THUỘC TÍNH LOGIC/TRẠNG THÁI
    int aaa; 
    int mucdolevel;
    int tamdung;
    int diem;
    int gameover; 
    float x, y; 
    float dodaichudiem;
    float dodaidiemtucthoi;
    float d1, d2, d3, d4, d5; 
    float fxqua, fyqua; 
    
    // CÁC ĐỐI TƯỢNG LIÊN QUAN
    Ran ran;
    Qua* qua; 
    Bangxephang bangxephang;
    
    // TEXTURE ẢNH
    SDL_Texture *background;
    SDL_Texture *background1;
    SDL_Texture *co;
    SDL_Texture *nentamdung;
    SDL_Texture *nengameover;
    SDL_Texture *vungran;
    SDL_Texture *trolai;
    SDL_Texture *tieptuc;
    SDL_Texture *moc;
    SDL_Texture *daomoc;
    
    // TEXTURE CHỮ
    SDL_Texture *gamemoitext, *dokhotext, *xephangtext, *detext, *binhthuongtext, *khotext;
    SDL_Texture *gameovertext;
    SDL_Texture *scoretext;
    SDL_Texture *diemtext;
    SDL_Texture *diemtttext;
    SDL_Texture *h1text, *h2text, *h3text, *h4text, *h5text;
    SDL_Texture *diemh1text, *diemh2text, *diemh3text, *diemh4text, *diemh5text;
    
    // SURFACE CHỮ
    SDL_Surface *gamemoisf; 
    SDL_Surface *dokhosf;
    SDL_Surface *xephangsf;
    SDL_Surface *desf;
    SDL_Surface *binhthuongsf;
    SDL_Surface *khosf;
    SDL_Surface *diemsf;
    SDL_Surface *gameoversf;
    SDL_Surface *scoresf;
    SDL_Surface *diemttsf;
    SDL_Surface *h1sf, *h2sf, *h3sf, *h4sf, *h5sf;
    SDL_Surface *diemh1sf, *diemh2sf, *diemh3sf, *diemh4sf, *diemh5sf;

    // MÀU
    SDL_Color Color = {144, 255, 73, 255}; 
    SDL_Color Color1= {80, 230, 80, 255};
    SDL_Color mauChuGO = {47, 93, 47, 255};
    SDL_Color mau = {0, 0, 0, 155};

    // NHẠC
    MIX_Audio *nhacnenA;
    MIX_Audio *nhacanA;
    MIX_Audio *nhacchetA;
    MIX_Track *nhacnenT;
    SDL_PropertiesID gMusicProps = 0;
    
public:
    // CONSTRUCTOR
    menu(SDL_Renderer *Renderer): ran(Renderer){
        // Khởi tạo thuộc tính
        ran.setDodairanbandau(4); 
        ran.setBuoc(40.0f); 
        ran.setMucthoigian(1);
        ran.setNhanphim(1);
        bangxephang.docfile();
        bangxephang.capnhatbangphu();
        aaa = 1;
        mucdolevel = 2;
        tamdung = 0;
        diem = 0;
        gameover = 0;
        x =240;
        y = 120;
        dodaichudiem = 0;
        dodaidiemtucthoi = 0;

        // Tải Texture ảnh
        background = IMG_LoadTexture(Renderer, "pictures/menu2.png");
        background1 = IMG_LoadTexture(Renderer, "pictures/bgmenu1.png");
        co = IMG_LoadTexture(Renderer, "pictures/co1.png");
        trolai = IMG_LoadTexture(Renderer, "pictures/trolai1-removebg-preview.png");
        tieptuc = IMG_LoadTexture(Renderer, "pictures/tieptuc-removebg-preview.png");
        nentamdung = IMG_LoadTexture(Renderer, "pictures/1-removebg-preview.png");
        nengameover = IMG_LoadTexture(Renderer, "pictures/1-removebg-preview.png");
        moc=IMG_LoadTexture(Renderer ,"pictures/moc-removebg-preview.png");
        daomoc=IMG_LoadTexture(Renderer ,"pictures/daomoc-removebg-preview.png");

        // Random quả đầu tiên
        qua = nullptr;
        randomloaiqua();
        
        // Tạo chữ
        const char *gamemoi = "New Game";
        const char *dokho = "Level";
        const char *xephang = "Ranking";
        const char *de = "Easy";
        const char *binhthuong = "Normal";
        const char *kho = "Hard";
        const char *chu_gameover = "Game Over";
        
        // TẠO SURFACE
        gamemoisf = TTF_RenderText_Blended(Font, gamemoi, strlen(gamemoi), Color);
        dokhosf = TTF_RenderText_Blended(Font, dokho, strlen(dokho), Color);
        xephangsf = TTF_RenderText_Blended(Font, xephang, strlen(xephang), Color);
        desf = TTF_RenderText_Blended(Font, de, strlen(de), Color1);
        binhthuongsf = TTF_RenderText_Blended(Font, binhthuong, strlen(binhthuong), Color1);
        khosf = TTF_RenderText_Blended(Font, kho, strlen(kho), Color1);
        gameoversf = TTF_RenderText_Blended(Font, chu_gameover, strlen(chu_gameover), mauChuGO);
        scoresf = TTF_RenderText_Blended(Font, "SCORE:", 6, mau);
        h1sf = TTF_RenderText_Blended(Font, "#1:", 3, Color1);
        h2sf = TTF_RenderText_Blended(Font, "#2:", 3, Color1);
        h3sf = TTF_RenderText_Blended(Font, "#3:", 3, Color1);
        h4sf = TTF_RenderText_Blended(Font, "#4:", 3, Color1);
        h5sf = TTF_RenderText_Blended(Font, "#5:", 3, Color1);
        
        // TẢI SURFACE THÀNH TEXTTURE;
        gamemoitext = SDL_CreateTextureFromSurface(Renderer, gamemoisf);
        SDL_DestroySurface(gamemoisf);
        dokhotext = SDL_CreateTextureFromSurface(Renderer, dokhosf);
        SDL_DestroySurface(dokhosf);
        xephangtext = SDL_CreateTextureFromSurface(Renderer, xephangsf);
        SDL_DestroySurface(xephangsf);
        detext = SDL_CreateTextureFromSurface(Renderer, desf);
        SDL_DestroySurface(desf);
        binhthuongtext = SDL_CreateTextureFromSurface(Renderer, binhthuongsf);
        SDL_DestroySurface(binhthuongsf);
        khotext = SDL_CreateTextureFromSurface(Renderer, khosf);
        SDL_DestroySurface(khosf);
        gameovertext = SDL_CreateTextureFromSurface(Renderer, gameoversf);
        SDL_DestroySurface(gameoversf);
        scoretext = SDL_CreateTextureFromSurface(Renderer, scoresf);
        SDL_DestroySurface(scoresf);
        h1text = SDL_CreateTextureFromSurface(Renderer, h1sf);
        SDL_DestroySurface(h1sf);
        h2text = SDL_CreateTextureFromSurface(Renderer, h2sf);
        SDL_DestroySurface(h2sf);
        h3text = SDL_CreateTextureFromSurface(Renderer, h3sf);
        SDL_DestroySurface(h3sf);
        h4text = SDL_CreateTextureFromSurface(Renderer, h4sf);
        SDL_DestroySurface(h4sf);
        h5text = SDL_CreateTextureFromSurface(Renderer, h5sf);
        SDL_DestroySurface(h5sf);

        // TẢI NHẠC
        nhacnenT = MIX_CreateTrack(Mix);
        nhacnenA = MIX_LoadAudio(Mix, "nhacnen2.mp3", 1);
        gMusicProps = SDL_CreateProperties();
        if (gMusicProps) {
            SDL_SetNumberProperty(gMusicProps, MIX_PROP_PLAY_LOOPS_NUMBER, -1);
        }
        MIX_SetTrackAudio(nhacnenT, nhacnenA);

        nhacanA = MIX_LoadAudio(Mix, "nhacanqua.mp3", 1);
        nhacchetA = MIX_LoadAudio(Mix, "gameover2.mp3", 1);
    }

    // DESTRUCTOR
    ~menu(){
        SDL_DestroyTexture(background);
        SDL_DestroyTexture(background1);
        SDL_DestroyTexture(gamemoitext);
        SDL_DestroyTexture(dokhotext);
        SDL_DestroyTexture(xephangtext);
        SDL_DestroyTexture(detext);
        SDL_DestroyTexture(binhthuongtext);
        SDL_DestroyTexture(khotext);
        SDL_DestroyTexture(nentamdung);
        SDL_DestroyTexture(nengameover);
        SDL_DestroyTexture(tieptuc);
        SDL_DestroyTexture(vungran);
        SDL_DestroyTexture(trolai);
        SDL_DestroyTexture(moc);
        SDL_DestroyTexture(daomoc);
        SDL_DestroyTexture(co);
        SDL_DestroyTexture(h1text);
        SDL_DestroyTexture(h2text);
        SDL_DestroyTexture(h3text);
        SDL_DestroyTexture(h4text);
        SDL_DestroyTexture(h5text);
        SDL_DestroyTexture(diemh1text);
        SDL_DestroyTexture(diemh2text);
        SDL_DestroyTexture(diemh3text);
        SDL_DestroyTexture(diemh4text);
        SDL_DestroyTexture(diemh5text);
        if (qua) delete qua;
        if (nhacnenA) MIX_DestroyAudio(nhacnenA);
        if (nhacanA) MIX_DestroyAudio(nhacanA);
        if (nhacchetA) MIX_DestroyAudio(nhacchetA);
        if (nhacnenT) MIX_DestroyTrack(nhacnenT);
        if (gMusicProps) SDL_DestroyProperties(gMusicProps);
    }

    // PHƯƠNG THỨC KHỞI TẠO VÀ CẬP NHẬT
    
    // KHỞI TẠO RẮN VỀ VỊ TRÍ BAN ĐẦU
    void khoitaoran(vector<float> &toadox, vector<float> &toadoy){
        // Xóa tọa độ cũ
        toadox.clear();
        toadoy.clear();
        ran.getToadox_truoc().clear();
        ran.getToadoy_truoc().clear();
        
        // Thiết lập tọa độ ban đầu
        float xbandau = 240, ybandau = 120;
        for (int i = 0; i < ran.getDodairanbandau(); i++){
            toadox.push_back(xbandau);
            toadoy.push_back(ybandau);
            xbandau -= 40;
        }
        ran.setHuong(1);
        ran.setDodairan(ran.getDodairanbandau());
        gameover = 0;
        
        // Random quả mới nếu quả cũ đã bị ăn
        if (ran.getQuabian()){
            randomvitriqua();
        }
        ran.setQuabian(false);
    } 

    // RANDOM LOẠI QUẢ MỚI
    void randomloaiqua(){
        if (qua) {
            delete qua;
            qua = nullptr;
        }
        int loai = rand() % 3 + 1;
        if (loai == 1) qua = new QuaTao(Renderer);
        else if (loai == 2) qua = new QuaThit(Renderer);
        else qua = new QuaCam(Renderer);
    }
    
    // RANDOM VỊ TRÍ QUẢ MỚI
    void randomvitriqua(){
        do{
            qua->setXqua(rand()%(1250 - 70 + 1) + 70);
            qua->setYqua(rand()%(850 - 30 + 1) + 30);
            fxqua = qua->getXqua() *1.000;
            fyqua = qua->getYqua() *1.000;
        }while(qua->getXqua() % 40 != 0 || qua->getYqua() % 40 != 0 || ran.ktra_qua(fxqua, fyqua, ran.getToadox(), ran.getToadoy()) == 0);
    }

    // TẠO ẢNH ĐIỂM XẾP HẠNG
    void khoitaoanhxephang(){
        if (diemh1text) SDL_DestroyTexture(diemh1text);
        if (diemh2text) SDL_DestroyTexture(diemh2text);
        if (diemh3text) SDL_DestroyTexture(diemh3text);
        if (diemh4text) SDL_DestroyTexture(diemh4text);
        if (diemh5text) SDL_DestroyTexture(diemh5text);

        string diemh1 = to_string(bangxephang.xephangphu[0]);
        diemh1sf = TTF_RenderText_Blended(Font, diemh1.c_str(), strlen(diemh1.c_str()), mau);
        d1 = strlen(diemh1.c_str()) * 40;
        diemh1text = SDL_CreateTextureFromSurface(Renderer, diemh1sf);
        SDL_DestroySurface(diemh1sf);

        string diemh2 = to_string(bangxephang.xephangphu[1]);
        diemh2sf = TTF_RenderText_Blended(Font, diemh2.c_str(), strlen(diemh2.c_str()), mau);
        d2 = strlen(diemh2.c_str()) * 40;
        diemh2text = SDL_CreateTextureFromSurface(Renderer, diemh2sf);
        SDL_DestroySurface(diemh2sf);

        string diemh3 = to_string(bangxephang.xephangphu[2]);
        diemh3sf = TTF_RenderText_Blended(Font, diemh3.c_str(), strlen(diemh3.c_str()), mau);
        d3 = strlen(diemh3.c_str()) * 40;
        diemh3text = SDL_CreateTextureFromSurface(Renderer, diemh3sf);
        SDL_DestroySurface(diemh3sf);

        string diemh4 = to_string(bangxephang.xephangphu[3]);
        diemh4sf = TTF_RenderText_Blended(Font, diemh4.c_str(), strlen(diemh4.c_str()), mau);
        d4 = strlen(diemh4.c_str()) * 40;
        diemh4text = SDL_CreateTextureFromSurface(Renderer, diemh4sf);
        SDL_DestroySurface(diemh4sf);

        string diemh5 = to_string(bangxephang.xephangphu[4]);
        diemh5sf = TTF_RenderText_Blended(Font, diemh5.c_str(), strlen(diemh5.c_str()), mau);
        d5 = strlen(diemh5.c_str()) * 40;
        diemh5text = SDL_CreateTextureFromSurface(Renderer, diemh5sf);
        SDL_DestroySurface(diemh5sf);
    }

    // TẠO ẢNH ĐIỂM CHUNG (Điểm cuối game)
    void khoitaoanhdiem(){
        if (diemtext) SDL_DestroyTexture(diemtext);
        string diem_chu = to_string(diem);
        SDL_Color mau = {0, 0, 0, 155};
        dodaichudiem = strlen(diem_chu.c_str()) * 60;
        diemsf = TTF_RenderText_Blended(Font, diem_chu.c_str(), strlen(diem_chu.c_str()), mau);
        diemtext = SDL_CreateTextureFromSurface(Renderer, diemsf);
        SDL_DestroySurface(diemsf);
    }
    
    // TẠO ẢNH ĐIỂM TỨC THỜI (Trong khi chơi)
    void diemtucthoi(){
        if (diemtttext) SDL_DestroyTexture(diemtttext); 
        string diemtt_chu = to_string(diem);
        SDL_Color mau = {0, 0, 0, 155};
        dodaidiemtucthoi = strlen(diemtt_chu.c_str());
        diemttsf = TTF_RenderText_Blended(Font, diemtt_chu.c_str(), strlen(diemtt_chu.c_str()), mau);
        diemtttext = SDL_CreateTextureFromSurface(Renderer, diemttsf);
        SDL_DestroySurface(diemttsf);
    }
    
    // CẬP NHẬT BẢNG XẾP HẠNG
    void capnhatbangxephang(){
        bangxephang.capnhatdiem(diem); // Đưa điểm mới vào
        bangxephang.sapsep(); // Sắp xếp
        bangxephang.capnhatbangphu(); // Cập nhật mảng hiển thị
        khoitaoanhdiem(); // Cập nhật ảnh điểm cuối game
        khoitaoanhxephang(); // Cập nhật ảnh điểm xếp hạng
        bangxephang.ghifile(); // Ghi ra file
    }
    
    // PHƯƠNG THỨC XỬ LÝ SỰ KIỆN
    void sukien(){
        while (SDL_PollEvent(&Event)){
            SDL_ConvertEventToRenderCoordinates(Renderer, &Event);
            
            if (Event.type == SDL_EVENT_QUIT){
                running = false;
            }else if(Event.type == SDL_EVENT_KEY_DOWN){

                // XỬ LÝ PHÍM ĐIỀU HƯỚNG
                ran.sukienran(Event);

            }else if (Event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && Event.button.button == SDL_BUTTON_LEFT){
                float mx = Event.button.x;
                float my = Event.button.y;

                if (aaa == 2 && gameover < 2){ // Đang chơi game
                    if(tamdung == 0){
                        // TẠM DỪNG (Nút quay lại)
                        if (mx > -20 && mx < -20+150 && my > -10 && my < -10+80 ){
                            tamdung = 1;
                        }
                    } else { // Đang ở màn hình Tạm dừng
                        // QUAY LẠI MÀN CHÍNH (Nút trái)
                        if (mx > 360 && mx < 520 && my > 600 && my < 720 ){
                            aaa = 1;
                            tamdung = 0;
                            // Reset game logic
                            x = 240; y = 120;
                            khoitaoran(ran.getToadox(), ran.getToadoy());
                            diemtucthoi();
                            capnhatbangxephang(); // Cập nhật điểm/rank
                        // TIẾP TỤC (Nút phải)
                        }else if(mx > 850 && mx < 990 && my > 600 && my < 720 ){
                            tamdung = 0;
                        }
                    }
                }else if (aaa == 2 && gameover > 2){ // Màn hình GAME OVER
                    // QUAY LẠI MÀN CHÍNH (Nút trái)
                    if (mx > 360 && mx < 520 && my > 600 && my < 720 ){
                        aaa = 1; 
                        x = 240; y = 120;
                        capnhatbangxephang(); // Cập nhật điểm/rank
                        randomvitriqua();
                        diem = 0; diemtucthoi();
                    // CHƠI TIẾP (Nút phải)
                    }else if(mx > 850 && mx < 990 && my > 600 && my < 720 ){
                        aaa = 2;
                        x = 240; y = 120;
                        khoitaoran(ran.getToadox(), ran.getToadoy()); 
                        randomloaiqua();
                        randomvitriqua();
                        capnhatbangxephang(); // Cập nhật điểm/rank
                        diem = 0; diemtucthoi();
                    }
                }else if (aaa == 1){ // Main Menu
                    if (mx > 370 && mx < 920 && my > 110 && my < 310 ){ // New Game
                        aaa = 2;
                        khoitaoran(ran.getToadox(), ran.getToadoy());
                        diem = 0; diemtucthoi();
                        bangxephang.capnhatbangphu();
                        khoitaoanhdiem();
                        khoitaoanhxephang();
                    }else if (mx > 370 && mx < 920 && my > 390 && my < 530 ){ // Level
                        aaa = 3;
                    }else if (mx > 370 && mx < 920 && my > 630 && my < 800 ){ // Ranking
                        aaa = 4;
                        bangxephang.capnhatbangphu();
                        khoitaoanhxephang();
                    }
                }else if (aaa == 3){ // Menu Độ Khó (Level)
                    if (mx > -20 && mx < -20+150 && my > -10 && my < -10+80 ){ // Back
                        aaa = 1; 
                        bangxephang.capnhatbangphu();
                        khoitaoanhdiem();
                        khoitaoanhxephang();
                    }
                    else if(mx > 530 && mx < 530+250 && my > 330 && my < 330+120){ // Easy
                        ran.setMucthoigian(2);
                        mucdolevel=1;
                    }
                    else if(mx > 430 && mx < 430+450 && my > 470 && my < 470+120){ // Normal
                        ran.setMucthoigian(1);
                        mucdolevel=2;
                    }
                    else if(mx > 530 && mx < 530+250 && my > 620 && my < 620+120){ // Hard
                        ran.setMucthoigian(0.5);
                        mucdolevel=3;
                    }
                }else if (aaa == 4){ // Menu Xếp Hạng (Ranking)
                    if (mx > -20 && mx < -20+150 && my > -10 && my < -10+80 ){ // Back
                        aaa = 1; 
                    }
                }
            }
        }
    }
    

    // VẼ NỀN CỎ
    void veco(){
        SDL_RenderClear(Renderer);
        SDL_RenderTexture(Renderer, background1, nullptr, nullptr);
        SDL_FRect vitrico = {70, 30, 1220, 860};
        SDL_RenderTexture(Renderer, co, nullptr, &vitrico);
    }

    // HÌNH ẢNH TẠM DỪNG
    void hinhanhtamdung(){
        SDL_FRect vitrinentamdung = {70, 30, 1220, 860};
        SDL_RenderTexture(Renderer, nentamdung, nullptr, &vitrinentamdung);
        SDL_FRect vitriscore = {450, 300, 250, 180};
        SDL_RenderTexture(Renderer, scoretext, nullptr, &vitriscore);
        SDL_FRect vitridiem = {730, 300, dodaichudiem + 100, 180};
        SDL_RenderTexture(Renderer, diemtttext, nullptr, &vitridiem);
    }
    
    // HÌNH ẢNH NỀN GAME OVER
    void hinhanhnengameover(){
        SDL_FRect vitrinengameover = {70, 30, 1220, 860};
        SDL_RenderTexture(Renderer, nengameover, nullptr, &vitrinengameover);
        SDL_FRect vitriGO = {310, 200, 780, 280};
        SDL_RenderTexture(Renderer, gameovertext, nullptr, &vitriGO);
        SDL_FRect vitriscore ={500, 500, 200, 90};
        SDL_RenderTexture(Renderer, scoretext, nullptr, &vitriscore);
        SDL_FRect vitridiem = {720, 500, dodaichudiem, 90};
        SDL_RenderTexture(Renderer, diemtttext, nullptr, &vitridiem);
    }

    // HÌNH ẢNH RẮN CHẾT VÀ GAME OVER
    void game_over(int i){
        
        if(i < 120){
            if (i > 20 && i < 40 || i > 60 && i < 80 || i > 100){
                ran.veran(ran.getToadox_truoc(), ran.getToadoy_truoc(),ran.getDodairan());
            }
            SDL_FRect vitriback = {-20, -10, 110, 70}; 
            SDL_RenderTexture(Renderer, trolai, nullptr, &vitriback);
            qua->taoqua(qua->getXqua(), qua->getYqua());
            SDL_RenderPresent(Renderer);
            
        }else{
            ran.veran(ran.getToadox_truoc(), ran.getToadoy_truoc(),ran.getDodairan());
            khoitaoanhdiem();
            hinhanhnengameover();
            SDL_RenderPresent(Renderer);
        }
    }
    
    // HÌNH ẢNH GAME CHẠY (LOGIC CHÍNH)
    void hinhanh(){
        SDL_RenderClear(Renderer);
        
        if (tamdung){
            veco();
            qua->taoqua(qua->getXqua(), qua->getYqua());
            ran.veran(ran.getToadox(), ran.getToadoy(), ran.getDodairan());
            hinhanhtamdung();
            SDL_RenderPresent(Renderer);
        } else {
            veco();
            
            if(gameover > 2 ){
                if(gameover == 3)MIX_PlayAudio(Mix, nhacchetA);
                game_over(gameover);
            } else {
                // Vẽ rắn 
                if(gameover < 2 && ran.ktrakhongdamtuong(ran.getToadox(), ran.getToadoy())){
                    ran.veran(ran.getToadox(), ran.getToadoy(),ran.getDodairan());
                }

                // Vẽ thêm chi tiết khi đang chơi
                SDL_FRect vitriback = {-20, -10, 110, 70}; 
                SDL_RenderTexture(Renderer, trolai, nullptr, &vitriback); // Nút tạm dừng
                SDL_FRect vitridiemtt = {1120, 50, dodaidiemtucthoi * 40, 70};
                SDL_RenderTexture(Renderer, diemtttext, nullptr, &vitridiemtt); // Điểm số
                
                //Xử lý ăn quả
                fxqua = qua->getXqua() *1.000;
                fyqua = qua->getYqua() *1.000;
                ran.ktraquabian(ran.getToadox(), ran.getToadoy(), fxqua, fyqua);
                
                if(ran.getQuabian()){
                    MIX_PlayAudio(Mix, nhacanA);
                    diem += qua->getdiem();
                    randomloaiqua();
                    randomvitriqua();
                    ran.setQuabian(false);
                    diemtucthoi();
                }
                
                qua->taoqua(qua->getXqua(), qua->getYqua());
                SDL_RenderPresent(Renderer);
            }
        }
    }

    // XỬ LÝ LOGIC GAME CHÍNH
    void danhdau(){
        
        sukien(); // Xử lý sự kiện trước

        if (ran.ktrakhongdamtuong(ran.getToadox(), ran.getToadoy()) && ran.ktrakhongcanduoi(ran.getToadox(), ran.getToadoy(), x, y)){
            if (tamdung == 0){ // Chỉ di chuyển khi không tạm dừng
                if(gameover == 0){
                    ran.luutruoc(ran.getToadox(), ran.getToadoy()); 
                }
                ran.logicgame(ran.getToadox(), ran.getToadoy(), x, y);
            }
            hinhanh();
        }else {
            if (gameover < 100000){ 
                hinhanh();
                gameover += 1;
            } else { 
                ran.setHuong(1);
                x = 240; y = 120;
                khoitaoran(ran.getToadox(), ran.getToadoy());
                gameover = 0;
                capnhatbangxephang();
            }
        }
    }
    
    // PHƯƠNG THỨC GIAO DIỆN CHÍNH
    
    // GIAO DIỆN MÀN HÌNH CHÍNH (MAIN MENU)
    void mainmenu(){
        SDL_RenderClear(Renderer);
        SDL_RenderTexture(Renderer, background, nullptr, nullptr);
        SDL_FRect vitri1 = {360, 110, 650, 200};
        SDL_FRect vitri2 = {460, 320, 400, 200};
        SDL_FRect vitri3 = {370, 550, 580, 200};
        SDL_RenderTexture(Renderer, gamemoitext, nullptr, &vitri1);
        SDL_RenderTexture(Renderer, dokhotext, nullptr, &vitri2);
        SDL_RenderTexture(Renderer, xephangtext, nullptr, &vitri3);
        SDL_RenderPresent(Renderer);
    }
    
    // GIAO DIỆN ĐỘ KHÓ (LEVEL MENU)
    void menu2(){
        SDL_RenderClear(Renderer);
        SDL_RenderTexture(Renderer, background, nullptr, nullptr);
        SDL_FRect vitri1 ={460, 100, 400, 200};
        SDL_RenderTexture(Renderer, dokhotext, nullptr, &vitri1);
        
        // VẼ MÓC THEO MỨC ĐỘ
        if(mucdolevel == 1){
            SDL_FRect mocRect={180, 340, 500, 100};
            SDL_RenderTexture(Renderer, moc, nullptr, &mocRect);
            SDL_FRect daomocRect={625, 343, 500, 100};
            SDL_RenderTexture(Renderer, daomoc, nullptr, &daomocRect);
        }
        else if (mucdolevel == 2) {
            SDL_FRect mocRect={80, 480, 500, 100};
            SDL_RenderTexture(Renderer, moc, nullptr, &mocRect);
            SDL_FRect daomocRect={720, 483, 500, 100};
            SDL_RenderTexture(Renderer, daomoc, nullptr, &daomocRect);
        }
        else if (mucdolevel == 3) {
            SDL_FRect mocRect={180, 630, 500, 100};
            SDL_RenderTexture(Renderer, moc, nullptr, &mocRect);
            SDL_FRect daomocRect={628, 633, 500, 100};
            SDL_RenderTexture(Renderer, daomoc, nullptr, &daomocRect);
        }
        
        // VẼ CÁC CHỮ
        SDL_FRect vitriback = {-20, -10, 150 ,80};
        SDL_FRect deRect = {530, 330, 250, 120}; 
        SDL_FRect tbRect = {430, 470, 450, 120}; 
        SDL_FRect khoRect = {530, 620, 250, 120};
        SDL_RenderTexture(Renderer, detext, nullptr, &deRect);
        SDL_RenderTexture(Renderer, binhthuongtext, nullptr, &tbRect);
        SDL_RenderTexture(Renderer, khotext, nullptr, &khoRect);
        SDL_RenderTexture(Renderer, trolai, nullptr, &vitriback);
        SDL_RenderPresent(Renderer);
        
    }
    
    // GIAO DIỆN XẾP HẠNG (RANKING MENU)
    void menu3(){
        SDL_RenderClear(Renderer);
        SDL_RenderTexture(Renderer, background, nullptr, nullptr);
        SDL_FRect vitri1 = {460, 100, 500, 200};
        SDL_RenderTexture(Renderer, xephangtext, nullptr, &vitri1);
        SDL_FRect vitriback = {-20, -10, 150, 80};
        SDL_RenderTexture(Renderer, trolai, nullptr, &vitriback);
        
        // HẠNG
        SDL_FRect vitrih1 = {500, 300, 100, 80};
        SDL_RenderTexture(Renderer, h1text, nullptr, &vitrih1);
        SDL_FRect vitrih2 = {500, 400, 100, 80};
        SDL_RenderTexture(Renderer, h2text, nullptr, &vitrih2);
        SDL_FRect vitrih3 = {500, 500, 100, 80};
        SDL_RenderTexture(Renderer, h3text, nullptr, &vitrih3);
        SDL_FRect vitrih4 = {500, 600, 100, 80};
        SDL_RenderTexture(Renderer, h4text, nullptr, &vitrih4);
        SDL_FRect vitrih5 = {500, 700, 100, 80};
        SDL_RenderTexture(Renderer, h5text, nullptr, &vitrih5);
        
        // ĐIỂM
        SDL_FRect vitridiemh1 = {650, 300, d1, 80};
        SDL_RenderTexture(Renderer, diemh1text, nullptr, &vitridiemh1);
        SDL_FRect vitridiemh2 = {650, 400, d2, 80};
        SDL_RenderTexture(Renderer, diemh2text, nullptr, &vitridiemh2);
        SDL_FRect vitridiemh3 = {650, 500, d3, 80};
        SDL_RenderTexture(Renderer, diemh3text, nullptr, &vitridiemh3);
        SDL_FRect vitridiemh4 = {650, 600, d4, 80};
        SDL_RenderTexture(Renderer, diemh4text, nullptr, &vitridiemh4);
        SDL_FRect vitridiemh5 = {650, 700, d5, 80};
        SDL_RenderTexture(Renderer, diemh5text, nullptr, &vitridiemh5);
        SDL_RenderPresent(Renderer);
    }
    
    // VÒNG LẶP CHÍNH (MAIN GAME LOOP)
    void mainloop(){
        const int FPS_MUC_TIEU = 60;
        const Uint32 THOI_GIAN_KHUNG_HINH_MUC_TIEU = 1000 / FPS_MUC_TIEU;
        Uint32 thoidiembatdau;

        Uint64 demFPS = 0;
        Uint64 thoigiancuoiduoccapnhat = SDL_GetTicks();
        
        khoitaoran(ran.getToadox(), ran.getToadoy());
        diemtucthoi();
        khoitaoanhdiem();
        khoitaoanhxephang();
        MIX_PlayTrack(nhacnenT, gMusicProps);
        
        while(running){
            
            thoidiembatdau = SDL_GetTicks();
            
            if (aaa == 1){
                mainmenu();
                sukien();
            }else if(aaa == 2){
                danhdau(); 
                
                Uint64 thoigianhientai = SDL_GetTicks();
                demFPS++;
                
                if (thoigianhientai > thoigiancuoiduoccapnhat + 1000){
                    thoigiancuoiduoccapnhat = thoigianhientai;
                    Uint64 khunghinhthucte = demFPS; 
                    string tieude = "snake - FPS: " + to_string(khunghinhthucte);
                    SDL_SetWindowTitle(Window, tieude.c_str() );
                    demFPS = 0;
                }
            }else if(aaa == 3){
                menu2();
                sukien();
            }else if(aaa == 4){
                menu3();
                sukien();
            }

            Uint32 thoigiankhunghinh = SDL_GetTicks() - thoidiembatdau;
            
            if (thoigiankhunghinh < THOI_GIAN_KHUNG_HINH_MUC_TIEU) {
                SDL_Delay(THOI_GIAN_KHUNG_HINH_MUC_TIEU - thoigiankhunghinh); 
            }
        }
    }
};



// 7. HÀM MAIN
int main (){
    try {
        if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO ) < 0) {
            throw "SDL khong the khoi tao!";
        }
        if (TTF_Init() < 0) {
            throw "SDL_ttf khong the khoi tao!";
        }

        if (MIX_Init() < 0) {
            throw "SDL_ttf khong the khoi tao!";
        }

        Mix = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
        if (!Mix) {
             throw "Khong the tao Mix!";
        }
        
        Window = SDL_CreateWindow("snake", 1360, 920, SDL_WINDOW_RESIZABLE);
        if (!Window) {
             throw "Khong the tao Window!";
        }
        
        Renderer = SDL_CreateRenderer(Window, nullptr);
        SDL_SetRenderLogicalPresentation(Renderer , 1360, 920, SDL_LOGICAL_PRESENTATION_STRETCH);
        if (!Renderer) {
             throw "Khong the tao Renderer!";
        }
        
        Font = TTF_OpenFont("FredokaOne-Regular.ttf", 100);
        if (!Font) {
             throw "Khong the tai Font FredokaOne-Regular.ttf!";
        }
        
        srand(time(nullptr));
    
        menu snake(Renderer);
        
        snake.mainloop();
        
    } catch (const char* msg) {
        
        cerr << "Loi Nghiem Trong: " << msg << endl;
        
        if (Renderer) SDL_DestroyRenderer(Renderer);
        if (Window) SDL_DestroyWindow(Window);
        if (Font) TTF_CloseFont(Font);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
    
    if (Renderer) SDL_DestroyRenderer(Renderer);
    if (Window) SDL_DestroyWindow(Window);
    if (Font) TTF_CloseFont(Font);
    TTF_Quit();
    SDL_Quit();
    return 0;
}