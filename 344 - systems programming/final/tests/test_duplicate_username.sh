#!/bin/bash

SERVER_HOST=10.0.2.15     # Sunucu IP adresi (gerekirse değiştir)
SERVER_PORT=8080       # Sunucu portu
CLIENT_BIN=./client       # İstemci programının yolu

USERNAME="testuser42"

# Birinci istemci (başarılı olmalı)
{
    sleep 1
    echo "$USERNAME"
    sleep 1
    echo "/exit"
} | $CLIENT_BIN "$SERVER_HOST" "$SERVER_PORT" > client1_log.txt 2>&1 &
PID1=$!

# Kısa bir gecikme ile ikinci istemciyi aynı kullanıcı adıyla başlat (başarısız olmalı)
sleep 2
{
    sleep 1
    echo "$USERNAME"
    sleep 2
    echo "/exit"
} | $CLIENT_BIN "$SERVER_HOST" "$SERVER_PORT" > client2_log.txt 2>&1 &
PID2=$!

wait $PID1
wait $PID2

echo "Test tamamlandı. Sonuçlar:"
echo "Birinci istemci logu (client1_log.txt):"
grep ERROR client1_log.txt || echo "ERROR yok, giriş başarılı."
echo
echo "İkinci istemci logu (client2_log.txt):"
grep ERROR client2_log.txt

echo
echo "Beklenen: İkinci istemci '[ERROR] Username already taken. Choose another.' benzeri bir mesaj görmeli!"