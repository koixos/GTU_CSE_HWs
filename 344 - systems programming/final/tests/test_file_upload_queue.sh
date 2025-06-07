#!/bin/bash
SERVER_HOST=10.0.2.15     # Sunucu IP adresi (gerekirse değiştir)
SERVER_PORT=8080            # Sunucu portu
CLIENT_BIN=./client           # İstemci binary yolu
TESTFILE=./testfiles/test.txt # Gönderilecek dosyanın yolu (mevcut ve <3MB olmalı)
TARGET_USER=user01            # Alıcı kullanıcı adı (tüm kullanıcılar aynı hedefe gönderecek)
NUM_CLIENTS=10

# test dosyası yoksa oluştur
if [ ! -f "$TESTFILE" ]; then
    mkdir -p "$(dirname "$TESTFILE")"
    base64 /dev/urandom | head -c 1024 > "$TESTFILE"
fi

# Sunucuya önceden bir alıcı kullanıcıyı (user01) giriş yapıp bekletmek iyi olur
# YOKSA önce user01 ile ayrı bir terminalde bağlanıp bir odaya gir ve açık bırak!

run_client() {
    idx="$1"
    username="uploaduser$(printf "%02d" $idx)"
    {
        sleep 1
        echo "$username"
        sleep 1
        echo "/join testroom"
        sleep 1
        echo "/sendfile test.txt $TARGET_USER"
        sleep 5
        echo "/exit"
    } | $CLIENT_BIN "$SERVER_HOST" "$SERVER_PORT" > upload_log_$idx.txt 2>&1 &
    echo "Started upload client $username (log: upload_log_$idx.txt)"
}

for i in $(seq 1 $NUM_CLIENTS); do
    run_client "$i"
    sleep 0.2
done

echo "Tüm $NUM_CLIENTS upload istemcisi başlatıldı. Sunucu logunu ve istemci loglarını kontrol et!"

wait
echo "Tüm upload istemcileri işini bitirdi."