#!/bin/bash

SERVER_HOST=10.0.2.15     # Sunucu IP adresi (gerekirse değiştir)
SERVER_PORT=8080         # Sunucu portu (gerekirse değiştir)
CLIENT_BIN=./client       # İstemci programının yolu (gerekirse değiştir)
NUM_CLIENTS=30

# Otomatik kullanıcı adı üret
gen_username() {
    echo "user$(printf "%02d" $1)"
}

# Her istemciye bir terminalde komut gönderen fonksiyon
run_client() {
    idx="$1"
    username=$(gen_username "$idx")
    {
        sleep 1
        echo "$username"
        sleep 1
        echo "/join testroom"
        sleep 1
        echo "/broadcast Selam, ben $username"
        sleep 1
        echo "/whisper user01 Merhaba user01, ben $username"
        sleep 2
        echo "/exit"
    } | $CLIENT_BIN "$SERVER_HOST" "$SERVER_PORT"
}

# İstemcileri başlat (her biri arka planda)
for i in $(seq 1 $NUM_CLIENTS); do
    run_client "$i" > client_log_$i.txt 2>&1 &
    echo "Started client $i (log: client_log_$i.txt)"
    sleep 0.2  # Hepsini aynı anda başlatmak için azaltabilirsin
done

echo "Tüm $NUM_CLIENTS istemci başlatıldı. Test sürüyor..."

wait
echo "Tüm istemciler testini tamamladı."