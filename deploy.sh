#!/bin/bash
# Author: Kang Lin <kl222@126.com>

set -e

update_verion() {
    sed -i "s/^\!define PRODUCT_VERSION.*/\!define PRODUCT_VERSION \"${VERSION}\"/g" ${SOURCE_DIR}/Install/Install.nsi

    sed -i "s/<VERSION>.*</<VERSION>${VERSION}</g" ${SOURCE_DIR}/Update/update.xml
    sed -i "s/^version: '.*{build}'/version: '${VERSION}.{build}'/g" ${SOURCE_DIR}/appveyor.yml
    sed -i "s/  - set TransformCoordinate_VERSION=.*/  - set TransformCoordinate_VERSION=\"${VERSION}\"/g" ${SOURCE_DIR}/appveyor.yml
    sed -i "s/TransformCoordinate_VERSION:.*/TransformCoordinate_VERSION: \"${VERSION}\"/g" ${SOURCE_DIR}/appveyor.yml
    #sed -i "s/export VERSION=.*/export VERSION=\"${VERSION}\"/g" ${SOURCE_DIR}/.travis.yml
    sed -i "s/TransformCoordinate_VERSION:.*/TransformCoordinate_VERSION: \"${VERSION}\"/g" ${SOURCE_DIR}/.github/workflows/msvc.yml
    sed -i "s/TransformCoordinate_VERSION:.*/TransformCoordinate_VERSION: \"${VERSION}\"/g" ${SOURCE_DIR}/.github/workflows/build.yml
    sed -i "s/TransformCoordinate_VERSION:.*/TransformCoordinate_VERSION: \"${VERSION}\"/g" ${SOURCE_DIR}/.github/workflows/doxygen.yml
    sed -i "s/v[0-9]\+\.[0-9]\+\.[0-9]\+/${VERSION}/g" ${SOURCE_DIR}/README*.md

    sed -i "s/_[0-9]\+\.[0-9]\+\.[0-9]\+_/_${DEBIAN_VERSION}_/g" ${SOURCE_DIR}/README*.md

    sed -i "s/    SET(TransformCoordinate_VERSION \"${SEMVER_PATTERN}\"/    SET(TransformCoordinate_VERSION \"${DEBIAN_VERSION}\")/g" ${SOURCE_DIR}/CMakeLists.txt
    sed -i "s/    SET(TransformCoordinate_VERSION.*/    SET(TransformCoordinate_VERSION \"${DEBIAN_VERSION}\")/g" ${SOURCE_DIR}/App/CMakeLists.txt
    sed -i "s/TransformCoordinate_VERSION:.*/TransformCoordinate_VERSION: \"${DEBIAN_VERSION}\"/g" ${SOURCE_DIR}/.github/workflows/ubuntu.yml
    sed -i "s/TransformCoordinate_VERSION:.*/TransformCoordinate_VERSION: \"${DEBIAN_VERSION}\"/g" ${SOURCE_DIR}/.github/workflows/macos.yml
    sed -i "s/TransformCoordinate_VERSION:.*/TransformCoordinate_VERSION: \"${DEBIAN_VERSION}\"/g" ${SOURCE_DIR}/.github/workflows/mingw.yml
    sed -i "s/TransformCoordinate_VERSION:.*/TransformCoordinate_VERSION: \"${DEBIAN_VERSION}\"/g" ${SOURCE_DIR}/.github/workflows/android.yml
    sed -i "s/transformcoordinate (.*)/transformcoordinate (${DEBIAN_VERSION})/g" ${SOURCE_DIR}/debian/changelog
    sed -i "s/Version=.*/Version=${DEBIAN_VERSION}/g" ${SOURCE_DIR}/Package/share/applications/org.Rabbit.TransformCoordinate.desktop
    sed -i "s/transformcoordinate_[0-9]\+\.[0-9]\+\.[0-9]\+_amd64.deb/transformcoordinate_${DEBIAN_VERSION}_amd64.deb/g" ${SOURCE_DIR}/README*.md
    sed -i "s/[0-9]\+\.[0-9]\+\.[0-9]\+/${DEBIAN_VERSION}/g" ${SOURCE_DIR}/App/android/AndroidManifest.xml
    if [ -f ${SOURCE_DIR}/vcpkg.json ]; then
        sed -i "s/  \"version-string\":.*\"[0-9]\+\.[0-9]\+\.[0-9]\+\",/  \"version-string\": \"${DEBIAN_VERSION}\",/g" ${SOURCE_DIR}/vcpkg.json
    fi
    if [ -f ${SOURCE_DIR}/vcpkg/manifests/vcpkg.json ]; then
        sed -i "s/  \"version-string\":.*\"[0-9]\+\.[0-9]\+\.[0-9]\+\",/  \"version-string\": \"${DEBIAN_VERSION}\",/g" ${SOURCE_DIR}/vcpkg/manifests/vcpkg.json
    fi

    CHANGLOG_TMP=${SOURCE_DIR}/debian/changelog.tmp
    CHANGLOG_FILE=${SOURCE_DIR}/debian/changelog
    echo "transformcoordinate (${DEBIAN_VERSION}) unstable; urgency=medium" > ${CHANGLOG_FILE}
    echo "" >> ${CHANGLOG_FILE}
    echo "`git log --pretty=format:'    * %s' ${PRE_TAG}..HEAD`" >> ${CHANGLOG_FILE}
    echo "" >> ${CHANGLOG_FILE}
    echo " -- `git log --pretty=format:'%an <%ae>' HEAD^..HEAD`  `date --rfc-email`" >> ${CHANGLOG_FILE}

    sed -i "s/android:versionCode=.*android/android:versionCode=\"${MAJOR_VERSION}\" android/g"  ${SOURCE_DIR}/App/android/AndroidManifest.xml
}

# Function to create sed-safe pattern
sed_safe_pattern() {
    local pattern="$1"
    # Escape special chars for sed BRE
    echo "$pattern" | sed 's/\([][\.*^$+?(){}|/]\)/\\\1/g'
}

# 安全的 readlink 函数，兼容各种系统
safe_readlink() {
    local path="$1"
    if [ -L "$path" ]; then
        if command -v readlink >/dev/null 2>&1; then
            if readlink -f "$path" >/dev/null 2>&1; then
                readlink -f "$path"
            else
                readlink "$path"
            fi
        else
            ls -l "$path" | awk '{print $NF}'
        fi
    elif [ -e "$path" ]; then
        if command -v realpath >/dev/null 2>&1; then
            realpath "$path"
        else
            echo "$(cd "$(dirname "$path")" && pwd)/$(basename "$path")"
        fi
    else
        echo "$path"
    fi
}

init_value() {
    # Official SemVer 2.0.0 pattern. See: https://semver.org/
    SEMVER_PATTERN="v?(0|[1-9][0-9]*)\.(0|[1-9][0-9]*)\.(0|[1-9][0-9]*)(-((0|[1-9][0-9]*|[0-9]*[a-zA-Z-][0-9a-zA-Z-]*)(\.(0|[1-9][0-9]*|[0-9]*[a-zA-Z-][0-9a-zA-Z-]*))*))?(\+([0-9a-zA-Z-]+(\.[0-9a-zA-Z-]+)*))?"
    # [rpm version](https://docs.fedoraproject.org/en-US/packaging-guidelines/Versioning/)
    RPM_VERSION_PATTERN="[0-9]+\.[0-9]+\.[0-9]+[+._~^0-9A-Za-z]*"
    # SemVer, git, deb and rpm version pattern
    VERSION_PATTERN="v?[0-9]+\.[0-9]+\.[0-9]+([-+_~.^][0-9A-Za-z.-]*)?"
    TAG_RELEASE_PATTERN="v?(0|[1-9][0-9]*)\.(0|[1-9][0-9]*)\.(0|[1-9][0-9]*)"

    SOURCE_DIR=$(dirname $(safe_readlink $0))
    RabbitCommonBash=$RabbitCommon_ROOT/Script/RabbitCommon.sh
    if [ ! -f ${RabbitCommonBash} ]; then
        RabbitCommonBash=${SOURCE_DIR}/../RabbitCommon/Script/RabbitCommon.sh
    fi
    if [ -n "${RabbitCommonBash}" -a -f ${RabbitCommonBash} ]; then
        source "${RabbitCommonBash}"
        check_echo_color_with_tput
        check_git
    else
        echo "Please set environment variable \"RabbitCommon_ROOT\" to \"RabbitCommon\" install root"
        echo "Or install \"RabbitCommon\" from \"https://github.com/KangLin/RabbitCommon.git\" in the same directory as the project"
        exit 1
    fi

    CURRENT_VERSION=`git describe --tags --match "v*"`
    if [ -z "$CURRENT_VERSION" ]; then
        CURRENT_VERSION=`git rev-parse HEAD`
    fi

    PRE_TAG=`git tag --sort=-creatordate | grep -E "^${TAG_RELEASE_PATTERN}$" | head -n 1`

    COMMIT="OFF"
    DEPLOY="OFF"
    VERSION=""
    MESSAGE=""

    # Check if sed supports -E
    if sed -E 's/a/b/g' 1>/dev/null 2>/dev/null <<< "test"; then
        #echo "Using sed with -E flag"
        SED_CMD="sed -i -E"
    else
        #echo "Falling back to BRE pattern"
        # Convert pattern to BRE
        BRE_PATTERN=$(sed_safe_pattern "$VERSION_PATTERN")
        SED_CMD="sed -i"
        VERSION_PATTERN="$BRE_PATTERN"
    fi
}

show_value() {
    echo ""
    echo "= Configuration:"
    echo "  Commit: ${COMMIT}"
    echo "  Deploy: ${DEPLOY}"
    echo "  Version: ${VERSION:-not specified}"
    echo "  Message: ${MESSAGE:-not specified}"
    echo "  Remaining args: $@"
    echo ""
    echo "  DEBIAN_VERSION: $DEBIAN_VERSION"
    echo "  RPM_VERSION: $RPM_VERSION"
    echo "  MAJOR_VERSION: $MAJOR_VERSION"
    echo "  DATE_TIME_UTC: $DATE_TIME_UTC"
    echo ""
    echo "  CURRENT_VERSION: $CURRENT_VERSION"
    echo "  PRE_TAG: $PRE_TAG"
    echo "  SOURCE_DIR: $SOURCE_DIR"
    echo ""
    echo "  SED_CMD: $SED_CMD"
    echo "  VERSION_PATTERN: $VERSION_PATTERN"
    echo ""
}

# Display detailed usage information
usage_long() {
    cat << EOF
$(basename $0) - Deploy script

Update version and push to remote repository.

Usage: $0 [OPTION ...] [VERSION]

Options:
  Information:
    -h                      Show this help message
    -s                      Show current version

  Version Management:
    -u                      Auto-increment current version
    -v VERSION              Set specific version (e.g., v1.0.0, 2.1.0-beta)
    -m MESSAGE              Set release message (used with -u, -c, -t)

  Version + Git Operations:
    -c                      Update version and commit changes
    -t VERSION              Set version, create git tag, and push to remote

Version format:
  Follows Semantic Versioning (SemVer) 2.0.0
    See: https://semver.org/

  Format: [v]X.Y.Z[-prerelease][+build]

  Examples:
    v1.0.0               # Release version
    v2.0.0-dev           # Development Version
    2.1.0-beta           # Prerelease version
    v1.2.3-alpha+001     # With build metadata
    3.0.0-rc.1           # Release candidate

Examples:
  # Show current version
  $0
  $0 -s

  # Only update version with current version
  $0 -u
  $0 -u -m "Release note"

  # Only update version
  $0 v1.0.0-dev
  $0 -v v1.0.0-dev

  # Update version and commit code
  # With current version
  $0 -c
  $0 -c -m "Release note"
  # With specify Version
  $0 -c v1.0.0-dev
  $0 -c -m "Release note" v1.0.0-dev

  # Update version and deploy
  $0 -t v1.0.0

  # Update version and deploy with tag and message
  $0 -t v1.0.0 -m "Release note"

EOF
    exit 0
}

parse_with_getopts() {

    if [ $# -eq 0 ]; then
        echo "Current version: $CURRENT_VERSION"
        echo "Last release tag: $PRE_TAG"
        echo ""
        echo "Help:"
        echo "  $0 -h"
        exit 0
    fi

    OPTIONS="hcusv:t:m:"
    # 使用 getopts 解析选项
    # OPTIONS 含义：字符后无冒号表示无参数，字符后有冒号表示需要参数。
    # h: 无参数
    # v: 需要参数（冒号跟在后面）
    while getopts $OPTIONS opt; do
        case $opt in
            h)
                usage_long
                ;;
            s)
                echo "Current version: $CURRENT_VERSION"
                exit 0
                ;;
            u)
                VERSION=$CURRENT_VERSION
                ;;
            c)
                COMMIT="ON"
                ;;
            v)
                VERSION="$OPTARG"
                #echo "Version set to: $VERSION"
                ;;
            t)
                VERSION="$OPTARG"
                DEPLOY="ON"
                COMMIT="ON"
                #echo "Tag set to: $TAG"
                ;;
            m)
                MESSAGE="$OPTARG"
                #echo "Message set to: $MESSAGE"
                ;;
            \?)
                echo "Invalid option: -$OPTARG" >&2
                usage_long
                ;;
            :)
                echo "Option -$OPTARG requires an argument." >&2
                usage_long
                ;;
        esac
    done

    # 移动参数指针，跳过已处理的选项
    shift $((OPTIND-1))

    # 处理剩余的位置参数（作为 VERSION）
    if [ $# -gt 0 ] && [ -z "$VERSION" ]; then
        VERSION="$1"
        #echo "Version from positional argument: $VERSION"
        shift
    fi

    if [ -z "$VERSION" ]; then
        VERSION=$CURRENT_VERSION
    fi

    # 参数验证
    if [ -n "$VERSION" ]; then
        if [[ ! "$VERSION" =~ ^${VERSION_PATTERN}$ ]]; then
            echo_error "X Invalid SemVer format: $VERSION" >&2
            echo_error "  Expected format: [v]X.Y.Z[-prerelease][+build]" >&2
            echo_error "  See: https://semver.org/" >&2
            exit 1
        fi
    fi

    if [ -n "$VERSION" -a -z "$MESSAGE" ]; then
        MESSAGE="Release $VERSION"
    fi

    # Prepare Debian version (remove 'v' prefix)
    DEBIAN_VERSION="${VERSION#v}"
    #DEBIAN_VERSION=${VERSION/#v/}

    RPM_VERSION=${DEBIAN_VERSION//-/\_}

    # Get major version for versionCode
    MAJOR_VERSION=`echo ${DEBIAN_VERSION} | cut -d "." -f 1`

    # Record update time
    DATE_TIME_UTC=$(date -u +"%Y-%m-%d %H:%M:%S (UTC)")
}

check_chang_log() {
    echo "  - Modified change log ?"
    local content=$(<${SOURCE_DIR}/ChangeLog.md)
    if [[ $content =~ "$VERSION" ]]; then
        echo_success "    √ Modified in \"ChangeLog.md\""
    else
        echo_warn "    ! Warning: Don't include \"$VERSION\" in the file \"ChangeLog.md\""
    fi
}

create_tag() {
    if [ "$DEPLOY" = "ON" ]; then
        #PRE_TAG=`git tag --sort=-taggerdate | head -n 1`
        echo "= Current version: $CURRENT_VERSION"
        echo "  Latest release tag: ${PRE_TAG:-none}"
        echo "  New tag version: $VERSION"
        echo "  Message: $MESSAGE"
        echo ""
        echo "Please verify:"
        echo "  - Test is ok ?"
        echo "  - Translations updated ?"
        check_chang_log
        echo ""

        read -t 60 -p "? Deploy? (y/N): " INPUT
        if [ "${INPUT:-N}" != "Y" ] && [ "${INPUT:-N}" != "y" ]; then
            echo_error "X Deployment cancelled"
            exit 0
        fi

        echo ""

        # Remove existing tag if it exists
        if git rev-parse "$VERSION" >/dev/null 2>&1; then
            echo "= Tag $VERSION already exists, deleting ......"
            git tag -d "$VERSION"
            echo_success "√ Successfully delete tag $VERSION"
            echo ""
        fi

        # Create new tag
        echo "= Creating tag: $VERSION ......"
        git tag -a "$VERSION" -m "${MESSAGE}"
        echo_success "√ Tag created: $VERSION"
        echo ""
    fi
}

commit_code() {
    echo "= Commit code ......"
    git add .

    # Commit if there are changes
    if ! git diff --cached --quiet; then
        git commit -m "$MESSAGE"
        echo_success "√ Changes committed"
    else
        echo_error "X No changes to commit"
        exit 1
    fi
}

push_remote_repository() {
    if [ "$DEPLOY" = "ON" ]; then
        echo "= Push to remote repository ......"

        # Update tag (delete and recreate)
        if git rev-parse "$VERSION" >/dev/null 2>&1; then
            git tag -d "$VERSION"
        fi
        git tag -a "$VERSION" -m "$MESSAGE"

        # Push commits and tags
        git push origin HEAD
        git push origin "$VERSION"

        echo_success "√ Push to remote repository successfully!"
    fi
}

# Start program
init_value

# 执行解析函数
parse_with_getopts "$@"

show_value

create_tag

echo "= Update version to $VERSION ......"

update_verion

echo_success "√ Version updated to $VERSION successfully!"
#echo "  Time: $DATE_TIME_UTC"
echo ""

if [ "$COMMIT" = "ON" ]; then
    commit_code
fi

push_remote_repository
