import cls from "@/utils/cls"

import styles from "./Settings.module.css"

interface SettingsProps {
	className?: string
}

export const Settings = ({
	className = ""
}: SettingsProps): JSX.Element => (
	<div className={cls(styles.settings, className)}>Settings</div>
)
