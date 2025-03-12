Name:           FlappyBird
Version:        2.0
Release:        1%{?dist}
Summary:        A simple FlappyBird game
Group:          Games/Entertainment

License:        GPLv3+
URL:            https://github.com/nastezzzi/MyGitProject/Galimullina_rpmbuild_FlappyBird
Source0:        FlappyBird-2.0.tar

BuildRequires:  gcc-c++
BuildRequires:  ncurses-devel
Requires:       ncurses

%description
A simple FlappyBird game implemented in C++.

%prep
%setup -q

%build
make %{?_smp_mflags}

%install
mkdir -p %{buildroot}/usr/local/bin
install -m 755 FlappyBird %{buildroot}/usr/local/bin/FlappyBird

%files
/usr/local/bin/FlappyBird

%changelog
* Mon Mar 12 2025 Anastasia Galimullina <nastezzzi03.mail@ru> - 2.0-1
- Initial package.

