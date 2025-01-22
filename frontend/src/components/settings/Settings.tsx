import styles from "./Settings.module.css"
import { Button, Link, Typography, Dropdown, DropdownItem, Popup } from "@/components/UI"
import cls from "@/utils/cls"
import { useState } from "react"
import { ReactSVG } from "react-svg"
import { useUserStore } from "@/state/user"

interface SettingsProps {
	className?: string
}

const data_settings = [
	{
		text: "Theme",
		icon: "/moon.svg",
		icon2: "/sun.svg"
	}
]

const data_account = [
	{
		text: "Logout",
		icon: "/logout.svg"
	},
	{
		text: "Help",
		icon: "/help.svg"
	}
]

type ThemeType =
	| "light"
	| "dark"

export const Settings = ({
	className = ""
}: SettingsProps): JSX.Element => {
	const [dropdownAccountState, setDropdownState] = useState<boolean>(false)
	const [dropdownSettingsState, setDropdownSettingsState] = useState<boolean>(false)
	const [themeState, setThemeState] = useState<ThemeType>("light")
    const [popupState, setPopupState] = useState<boolean>(false)
    const username = useUserStore((state) => state.username)
    const login = useUserStore((state) => state.login)

	const onAccountClick = () => {
		setDropdownState((prev: boolean): boolean => !prev)
	}

    const onLoginClick = () => {
        navigator.clipboard.writeText(`@${login}`)
        setPopupState(true)
        setTimeout(() => {
            setPopupState(false)
        }, 5000)
    }

	const onSettingsClick = () => {
		setDropdownSettingsState((prev: boolean): boolean => !prev)
	}

	const onThemeChange = (e: React.MouseEvent) => {
		e.preventDefault()
		if (themeState === "light") {
			setThemeState("dark")
		} else if (themeState === "dark") {
			setThemeState("light")
		}
	}

	return (
		<div className={cls(styles.settings, className)}>
			<Dropdown dropdownState={dropdownAccountState} className={cls(styles.dropdown, styles.dropdown_account)}>
				{ data_account.map((item, index: number): JSX.Element => (
					<DropdownItem className={styles.dropdown_item} key={index}>
						<Link href="/login">
							<>
								<ReactSVG
									className={cls(styles.icon)}
									src={item.icon}
								/>
								<span>{item.text}</span>
							</>
						</Link>
					</DropdownItem>
				)) }
			</Dropdown>
			<Button variant="icon" className={styles.button} onClick={onAccountClick}>
				<ReactSVG
					className={cls(styles.icon, styles.icon_account)}
					src="/account.svg"
				/>
			</Button>
			<Typography tag="h2" variant="title-4">{username}</Typography>
			<Button onClick={onLoginClick} variant="link" className={styles.login}><>@{login}</></Button>
            { popupState && <Popup direction="down_top">Copied!</Popup> }

			<Dropdown dropdownState={dropdownSettingsState} className={cls(styles.dropdown, styles.dropdown_settings)}>
				{ data_settings.map((_, index: number): JSX.Element => (
					<DropdownItem className={styles.dropdown_item} key={index}>
						<Link href="#" onClick={onThemeChange}>
							<>
								<ReactSVG
									className={cls(styles.icon)}
									src={themeState + ".svg"}
								/>
								<span>{themeState}</span>
							</>
						</Link>
					</DropdownItem>
				)) }
			</Dropdown>
			<Button variant="icon" className={styles.button} onClick={onSettingsClick}>
				<ReactSVG
					className={cls(styles.icon_search, styles.icon)}
					src="/settings.svg"
				/>
			</Button>
		</div>
	)
}
