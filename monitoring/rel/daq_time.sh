#!/bin/bash

# 로그 파일이 있는 디렉토리
LOG_DIR="/data/amore2test/LOG"

# 결과를 기록할 파일
OUTPUT_FILE="time.txt"

# "FADCDAQ_"로 시작하는 모든 파일 찾기
files=$(find "$LOG_DIR" -type f -name "FADCDAQ_*" | sort)

# 가장 최신 파일의 파일 번호 추출
latest_file=$(find "$LOG_DIR" -type f -name "FADCDAQ_*" | sort | tail -n 1)
latest_filename=$(basename -- "$latest_file")
latest_file_number=$(echo "$latest_filename" | sed 's/^FADCDAQ_//' | sed 's/\.log$//')

# 임시 파일 생성
TEMP_FILE=$(mktemp)

# 각 파일에 대해 처리
for file in $files; do
    # 파일 이름에서 경로와 확장자 제거하여 추출
    filename=$(basename -- "$file")
    file_number=$(echo "$filename" | sed 's/^FADCDAQ_//' | sed 's/\.log$//')

    # 최신 파일일 경우 항상 시간 계산 수행
    if [ "$file_number" != "$latest_file_number" ]; then
        # 이미 time.txt에 기록된 파일인지 확인
        if grep -q "^${file_number}" "$OUTPUT_FILE"; then
            #echo "$file_number 파일은 이미 처리되었습니다. 기존 정보를 유지합니다."
            # 기존 정보를 임시 파일에 복사
            grep "^${file_number}" "$OUTPUT_FILE" >> "$TEMP_FILE"
            continue
        fi
    fi

    # 'Live time : xx [s]'를 포함하는 줄 찾기
    live_time_line=$(grep 'Live time : ' "$file")

    # 시간 값을 추출
    time_value=$(echo "$live_time_line" | grep -oP 'Live time : \K\d+')

    # 결과 파일에 기록
    echo "${file_number}    ${time_value}" >> "$TEMP_FILE"
done

# 임시 파일을 최종 결과 파일로 이동
mv "$TEMP_FILE" "$OUTPUT_FILE"

# 임시 파일 삭제
rm -f "$TEMP_FILE"

echo "시간 추출이 완료되었습니다. 결과는 $OUTPUT_FILE 파일에 기록되었습니다."
